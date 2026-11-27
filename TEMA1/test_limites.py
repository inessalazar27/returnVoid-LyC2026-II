"""
Casos límite del AFD (validadorADF.validarEtiqueta).
Regla formal: [A-Z][a-z]+[0-9]

Uso (desde la carpeta TEMA1):
    python test_limites.py                 # imprime la tabla
    python test_limites.py --guardar       # además escribe reporte_limites.md

Código de salida: 0 si todos los casos coinciden con lo esperado, 1 si no.
"""
import sys
from validadorADF import validarEtiqueta
import sys
sys.stdout.reconfigure(encoding="utf-8")

# (cadena, esperado_acepta, descripción)
CASOS = [
    ("Get1", True, "caso base"),
    ("Run2", True, "caso base"),
    ("Read9", True, "caso base"),
    ("Ab1", True, "mínimo válido: 1 minúscula"),
    ("Get0", True, "dígito 0"),
    ("Gaaaaaaaaaaaaaaaa1", True, "muchas minúsculas"),
    ("get1", False, "empieza con minúscula"),
    ("Get", False, "sin dígito final"),
    ("Get12", False, "dos dígitos"),
    ("Get_1", False, "guion bajo fuera del alfabeto"),
    ("G1", False, "falta minúscula"),
    ("A1", False, "falta minúscula"),
    ("GET1", False, "mayúsculas donde van minúsculas"),
    ("", False, "cadena vacía"),
    ("Ñandú1", False, "Ñ fuera de A-Z"),
    ("Gét1", False, "tilde fuera de a-z"),
    ("Ge1t", False, "dígito en medio"),
    ("Get1 ", False, "espacio final"),
    ("Get1\n", False, "salto de línea final"),
    ("Get\u0663", False, "dígito arábigo-índico (U+0663)"),
]

def visible(c):
    return repr(c) if (c == "" or not c.isprintable() or c != c.strip()) else c

def main():
    filas, fallos = [], 0
    for cadena, esperado, desc in CASOS:
        r = validarEtiqueta(cadena)
        ok = r["valida"] == esperado
        fallos += (not ok)
        filas.append((visible(cadena), "acepta" if esperado else "rechaza",
                      "acepta" if r["valida"] else "rechaza",
                      r["estado_final"], "OK" if ok else "FALLA", desc))
    enc = ("Cadena", "Esperado", "Obtenido", "Estado final", "Resultado", "Caso")
    print(" | ".join(enc)); print("-" * 80)
    for f in filas: print(" | ".join(f))
    print(f"\n{len(CASOS) - fallos}/{len(CASOS)} casos coinciden con lo esperado")

    if "--guardar" in sys.argv:
        with open("reporte_limites.md", "w", encoding="utf-8") as fh:
            fh.write("| " + " | ".join(enc) + " |\n|" + "---|" * len(enc) + "\n")
            for f in filas:
                fh.write("| " + " | ".join(x.replace("|", "\\|") for x in f) + " |\n")
            fh.write(f"\n**{len(CASOS) - fallos}/{len(CASOS)} casos coinciden.**\n")
        print("Reporte guardado en reporte_limites.md", file=sys.stderr)
    return 0 if fallos == 0 else 1

if __name__ == "__main__":
    sys.exit(main())
