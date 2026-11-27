#!/usr/bin/env python3
"""Script de demostracion del reto: pasar las salidas de un LLM por el AFD.

Parte 3 del reto (Integrante 3).

Flujo:
  1. Toma las etiquetas de salidas_llm.py.
  2. Las pasa una por una por el motor del AFD (validadorADF.py).
  3. Compara el resultado con la regla formal.
  4. Opcional: toma las correcciones que propuso el LLM y las revalida.

Regla formal: [A-Z][a-z]+[0-9]

Uso:
    python pruebas_llm.py
    python pruebas_llm.py --json
    python pruebas_llm.py --sin-correccion
    python pruebas_llm.py --verbose
    python pruebas_llm.py --guardar resultados_llm.json

Codigo de salida: 0 si todos los casos coinciden con lo esperado, 1 si no.
"""

from __future__ import annotations

import argparse
import importlib
import json
import sys
import unicodedata
from dataclasses import dataclass
from pathlib import Path
import sys
sys.stdout.reconfigure(encoding="utf-8")

_DIRECTORIO = Path(__file__).resolve().parent
if str(_DIRECTORIO) not in sys.path:
    sys.path.insert(0, str(_DIRECTORIO))

from salidas_llm import CORRECCIONES_LLM, REGLA, SALIDAS_LLM  # noqa: E402

ANCHO_MOTIVO = 64


# ---------------------------------------------------------------------------
# Motor del AFD (Integrante 2)
# ---------------------------------------------------------------------------
# El motor vive en validadorADF.py y expone:
#     validarEtiqueta(cadena) -> {
#         "valida": bool,
#         "motivo": str,
#         "estado_final": str,
#         ...
#     }
#
# Ese archivo NO se modifica: solo se importa. Si todavia no existe, se usa un
# STUB local solo para poder desarrollar y demostrar el script. El reporte
# avisa cuando se usa el stub.

MODULOS_MOTOR = ("validadorADF", "afd_validador")
FUNCIONES_MOTOR = (
    "validarEtiqueta",
    "validar_etiqueta",
    "validar_cadena",
    "validar",
)


def _cargar_motor():
    """Busca el motor del AFD entre los archivos conocidos del equipo."""
    modulos_encontrados: list[str] = []

    for nombre_modulo in MODULOS_MOTOR:
        try:
            modulo = importlib.import_module(nombre_modulo)
        except ModuleNotFoundError as exc:
            if exc.name != nombre_modulo:
                raise
            continue

        modulos_encontrados.append(nombre_modulo)
        for nombre_funcion in FUNCIONES_MOTOR:
            funcion = getattr(modulo, nombre_funcion, None)
            if callable(funcion):
                return funcion, f"{nombre_modulo}.py -> {nombre_funcion}()"

    if modulos_encontrados:
        raise SystemExit(
            "Se encontro " + ", ".join(modulos_encontrados) + " pero no expone "
            "ninguna de estas funciones: " + ", ".join(FUNCIONES_MOTOR) + ".\n"
            "Revisa el nombre con el Integrante 2 o agrega el alias aqui."
        )
    return None, "STUB local (no se encontro el motor del AFD)"


_validar_motor, MOTOR = _cargar_motor()


# ---------------------------------------------------------------------------
# Utilidades
# ---------------------------------------------------------------------------
def visible(cadena: str) -> str:
    """Version imprimible y ASCII de una cadena (segura en consolas Windows)."""
    if cadena == "":
        return "(vacia)"
    partes: list[str] = []
    ultimo = len(cadena) - 1
    for posicion, caracter in enumerate(cadena):
        if caracter == " " and posicion in (0, ultimo):
            # Un espacio al borde es invisible: se muestra explicito.
            partes.append("\\u0020")
        elif 32 <= ord(caracter) < 127:
            partes.append(caracter)
        elif caracter == "\n":
            partes.append("\\n")
        elif caracter == "\t":
            partes.append("\\t")
        else:
            partes.append(f"\\u{ord(caracter):04x}")
    return "".join(partes)


def _ascii(texto: str) -> str:
    """Version ASCII para consolas Windows: quita tildes y escapa el resto."""
    normalizado = unicodedata.normalize("NFKD", texto)
    sin_marcas = "".join(
        caracter for caracter in normalizado if not unicodedata.combining(caracter)
    )
    reemplazos = {"\n": "\\n", "\r": "\\r", "\t": "\\t"}
    plano = "".join(reemplazos.get(caracter, caracter) for caracter in sin_marcas)
    return plano.encode("ascii", "backslashreplace").decode("ascii")


def _motivo_visible(motivo: str, cadena: str) -> str:
    """Escapa el caracter citado por el AFD y deja el resto del motivo en ASCII.

    El motor cita el caracter problematico entre comillas: 'carácter '{c}''.
    Si {c} no es ASCII, se muestra como \\uXXXX en lugar de perderlo al quitar
    las tildes del mensaje.
    """
    for caracter in cadena:
        if ord(caracter) > 127:
            motivo = motivo.replace(f"'{caracter}'", f"'{visible(caracter)}'")
    return _ascii(motivo)


def _normalizar(resultado) -> tuple[bool | None, str]:
    """Acepta el contrato (aceptada, motivo) y variantes razonables."""
    if isinstance(resultado, bool):
        return resultado, ""

    if isinstance(resultado, (tuple, list)) and resultado:
        aceptada = bool(resultado[0])
        motivo = resultado[1] if len(resultado) > 1 else ""
        return aceptada, "" if motivo is None else str(motivo)

    if isinstance(resultado, dict):
        for clave in ("aceptada", "valida", "ok", "resultado"):
            if clave in resultado:
                aceptada = bool(resultado[clave])
                for clave_motivo in ("motivo", "razon", "error", "mensaje"):
                    if clave_motivo in resultado:
                        return aceptada, str(resultado[clave_motivo] or "")
                return aceptada, ""

    for atributo in ("aceptada", "valida", "ok"):
        if hasattr(resultado, atributo):
            motivo = getattr(resultado, "motivo", "") or ""
            return bool(getattr(resultado, atributo)), str(motivo)

    raise TypeError(
        "El motor devolvio un tipo que no se reconoce: "
        f"{type(resultado).__name__}. Se esperaba (aceptada, motivo)."
    )


def _stub(cadena: str) -> tuple[bool, str]:
    """AFD de referencia SOLO para desarrollar sin el motor del Integrante 2.

    Estados:
        q0  inicial
        q1  leyo la mayuscula
        q2  leyo una o mas minusculas
        q3  final, leyo el digito

    Cualquier transicion no definida rechaza (estado de rechazo implicito).
    Este stub no sustituye a validadorADF.py.
    """
    estado = "q0"
    for posicion, caracter in enumerate(cadena):
        if estado == "q0" and "A" <= caracter <= "Z":
            estado = "q1"
        elif estado in ("q1", "q2") and "a" <= caracter <= "z":
            estado = "q2"
        elif estado == "q2" and "0" <= caracter <= "9":
            estado = "q3"
        else:
            return (
                False,
                f"caracter '{visible(caracter)}' en {estado} "
                f"(posicion {posicion}) no tiene transicion",
            )

    if estado == "q3":
        return True, ""
    if estado == "q0":
        return False, "cadena vacia: no hay caracter en q0"
    if estado == "q1":
        return False, "fin de cadena en q1: falta al menos una minuscula"
    return False, "fin de cadena en q2: falta el digito final"


def validar(cadena: str) -> tuple[bool | None, str]:
    """Valida una cadena con el motor real o con el stub."""
    if _validar_motor is not None:
        try:
            return _normalizar(_validar_motor(cadena))
        except Exception as exc:  # un caso roto no debe tumbar todo el reporte
            return None, f"ERROR del motor: {type(exc).__name__}: {exc}"
    return _stub(cadena)


# ---------------------------------------------------------------------------
# Ejecucion de los casos
# ---------------------------------------------------------------------------
@dataclass(frozen=True)
class Resultado:
    cadena: str
    esperado: bool
    categoria: str
    nota: str
    aceptada: bool | None
    motivo: str

    @property
    def ok(self) -> bool:
        return self.aceptada is not None and self.aceptada == self.esperado


def ejecutar_casos() -> list[Resultado]:
    resultados: list[Resultado] = []
    for salida in SALIDAS_LLM:
        cadena = salida["cadena"]
        aceptada, motivo = validar(cadena)
        resultados.append(
            Resultado(
                cadena=cadena,
                esperado=bool(salida["esperado"]),
                categoria=str(salida["categoria"]),
                nota=str(salida["nota"]),
                aceptada=aceptada,
                motivo=motivo,
            )
        )
    return resultados


def ejecutar_correcciones() -> list[dict]:
    """Pasa otra vez por el AFD las versiones corregidas por el LLM."""
    filas: list[dict] = []
    for original, corregida in CORRECCIONES_LLM.items():
        aceptada, motivo = validar(corregida)
        filas.append(
            {
                "original": original,
                "corregida": corregida,
                "aceptada": aceptada,
                "motivo": motivo,
            }
        )
    return filas


# ---------------------------------------------------------------------------
# Reporte en texto
# ---------------------------------------------------------------------------
def _imprimir_cabecera() -> None:
    print("=" * 66)
    print(" Reto LLM + AFD - script de pruebas (Integrante 3)")
    print(f" Regla: {REGLA}")
    print(f" Motor: {MOTOR}")
    print("=" * 66)
    if _validar_motor is None:
        print()
        print(" AVISO: no se encontro el motor del AFD; se usa el STUB local.")
        print("        Esto sirve para desarrollar, NO cuenta como prueba del")
        print("        motor del Integrante 2.")


def _imprimir_tabla(resultados: list[Resultado]) -> None:
    encabezados = ("#", "CADENA", "ESPERADO", "RESULTADO", "OK", "MOTIVO")
    filas: list[tuple[str, ...]] = []

    for numero, resultado in enumerate(resultados, 1):
        if resultado.aceptada is None:
            obtenido = "ERROR"
        else:
            obtenido = "acepta" if resultado.aceptada else "rechaza"

        motivo = "" if resultado.aceptada else (resultado.motivo or "-")
        motivo = _motivo_visible(motivo, resultado.cadena)
        if len(motivo) > ANCHO_MOTIVO:
            motivo = motivo[: ANCHO_MOTIVO - 3] + "..."

        filas.append(
            (
                str(numero),
                visible(resultado.cadena),
                "acepta" if resultado.esperado else "rechaza",
                obtenido,
                "si" if resultado.ok else "NO",
                motivo,
            )
        )

    anchos = [
        max([len(encabezados[i])] + [len(fila[i]) for fila in filas])
        for i in range(len(encabezados))
    ]

    print()
    print("  ".join(encabezados[i].ljust(anchos[i]) for i in range(len(encabezados))).rstrip())
    print("  ".join("-" * anchos[i] for i in range(len(encabezados))))
    for fila in filas:
        print("  ".join(fila[i].ljust(anchos[i]) for i in range(len(encabezados))).rstrip())


def _imprimir_resumen(resultados: list[Resultado], correcciones: list[dict]) -> None:
    total = len(resultados)
    aceptadas = sum(1 for r in resultados if r.aceptada is True)
    rechazadas = sum(1 for r in resultados if r.aceptada is False)
    errores = sum(1 for r in resultados if r.aceptada is None)
    coincidencias = sum(1 for r in resultados if r.ok)

    print()
    print("RESUMEN")
    print(f"  Casos totales:                 {total}")
    print(f"  Aceptadas por el AFD:          {aceptadas}")
    print(f"  Rechazadas por el AFD:         {rechazadas}")
    if errores:
        print(f"  Errores del motor:             {errores}")
    print(f"  Coinciden con lo esperado:     {coincidencias}/{total}")
    print(f"  Etiquetas del LLM atrapadas:   {rechazadas}")

    print()
    print("  Por categoria:")
    for categoria, etiqueta in (
        ("valida", "validas"),
        ("invalida", "invalidas"),
        ("ambigua", "ambiguas"),
    ):
        grupo = [r for r in resultados if r.categoria == categoria]
        if not grupo:
            continue
        grupo_acepta = sum(1 for r in grupo if r.aceptada is True)
        grupo_rechaza = sum(1 for r in grupo if r.aceptada is False)
        print(
            f"    {etiqueta:<10} {len(grupo):>3} casos | "
            f"acepta {grupo_acepta:>3} | rechaza {grupo_rechaza:>3}"
        )

    if correcciones:
        print()
        print("CICLO OPCIONAL: el LLM corrige las rechazadas y se revalidan")
        ancho_original = max(len(visible(f["original"])) for f in correcciones)
        ancho_corregida = max(len(visible(f["corregida"])) for f in correcciones)
        correctas = 0
        for fila in correcciones:
            if fila["aceptada"] is True:
                correctas += 1
                estado = "OK"
            elif fila["aceptada"] is None:
                estado = "ERROR"
            else:
                estado = "FALLA"
            print(
                f"  {visible(fila['original']):<{ancho_original}} -> "
                f"{visible(fila['corregida']):<{ancho_corregida}} {estado}"
            )
            if fila["aceptada"] is not True and fila["motivo"]:
                print(f"      motivo: {_motivo_visible(fila['motivo'], fila['corregida'])}")
        print(f"  Correcciones aceptadas: {correctas}/{len(correcciones)}")


def _imprimir_notas(resultados: list[Resultado]) -> None:
    ambiguas = [r for r in resultados if r.categoria == "ambigua"]
    if not ambiguas:
        return
    print()
    print("POR QUE LOS CASOS AMBIGUOS SON TRAMPA")
    for resultado in ambiguas:
        print(f"  {visible(resultado.cadena):<14} {_ascii(resultado.nota)}")


def _imprimir_detalle_rechazos(resultados: list[Resultado]) -> None:
    rechazados = [r for r in resultados if r.aceptada is False]
    if not rechazados:
        return
    ancho = max(len(visible(r.cadena)) for r in rechazados)
    print()
    print("DETALLE DE RECHAZOS (motivo completo del AFD)")
    for resultado in rechazados:
        print(
            f"  {visible(resultado.cadena):<{ancho}}  "
            f"{_motivo_visible(resultado.motivo, resultado.cadena)}"
        )


# ---------------------------------------------------------------------------
# Reporte en JSON (para el Integrante 4)
# ---------------------------------------------------------------------------
def _reporte_json(resultados: list[Resultado], correcciones: list[dict]) -> dict:
    return {
        "motor": MOTOR,
        "usando_stub": _validar_motor is None,
        "regla": REGLA,
        "resumen": {
            "total": len(resultados),
            "aceptadas": sum(1 for r in resultados if r.aceptada is True),
            "rechazadas": sum(1 for r in resultados if r.aceptada is False),
            "errores_motor": sum(1 for r in resultados if r.aceptada is None),
            "coincidencias": sum(1 for r in resultados if r.ok),
            "correcciones_total": len(correcciones),
            "correcciones_aceptadas": sum(
                1 for c in correcciones if c["aceptada"] is True
            ),
        },
        "casos": [
            {
                "cadena": r.cadena,
                "visible": visible(r.cadena),
                "categoria": r.categoria,
                "esperado": "acepta" if r.esperado else "rechaza",
                "obtenido": (
                    "error"
                    if r.aceptada is None
                    else ("acepta" if r.aceptada else "rechaza")
                ),
                "coincide": r.ok,
                "motivo": r.motivo,
                "nota": r.nota,
            }
            for r in resultados
        ],
        "correcciones": [
            {
                "original": c["original"],
                "original_visible": visible(c["original"]),
                "corregida": c["corregida"],
                "corregida_visible": visible(c["corregida"]),
                "aceptada": c["aceptada"],
                "motivo": c["motivo"],
            }
            for c in correcciones
        ],
    }


# ---------------------------------------------------------------------------
# Punto de entrada
# ---------------------------------------------------------------------------
def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Pasa las salidas de un LLM por el AFD validador de etiquetas."
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="imprime el reporte en JSON (para el Integrante 4)",
    )
    parser.add_argument(
        "--sin-correccion",
        action="store_true",
        help="omite el ciclo opcional de correccion del LLM",
    )
    parser.add_argument(
        "--verbose",
        "-v",
        action="store_true",
        help="explica por que los casos ambiguos son trampa",
    )
    parser.add_argument(
        "--guardar",
        metavar="ARCHIVO",
        help="guarda el reporte JSON en ARCHIVO (UTF-8) para el Integrante 4",
    )
    args = parser.parse_args(argv)

    resultados = ejecutar_casos()
    correcciones = [] if args.sin_correccion else ejecutar_correcciones()
    reporte = _reporte_json(resultados, correcciones)

    if args.json:
        print(json.dumps(reporte, ensure_ascii=True, indent=2))
    else:
        _imprimir_cabecera()
        _imprimir_tabla(resultados)
        _imprimir_resumen(resultados, correcciones)
        if args.verbose:
            _imprimir_notas(resultados)
            _imprimir_detalle_rechazos(resultados)

    if args.guardar:
        ruta = Path(args.guardar)
        ruta.parent.mkdir(parents=True, exist_ok=True)
        ruta.write_text(
            json.dumps(reporte, ensure_ascii=False, indent=2),
            encoding="utf-8",
        )
        print(f"Reporte JSON guardado en: {ruta}", file=sys.stderr)

    return 0 if all(r.ok for r in resultados) else 1


if __name__ == "__main__":
    raise SystemExit(main())
