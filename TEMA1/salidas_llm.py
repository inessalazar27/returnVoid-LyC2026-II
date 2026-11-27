"""Salidas de un LLM usadas como datos de prueba del validador de etiquetas.

Parte 3 del reto (Integrante 3): pedirle etiquetas a un LLM, guardarlas en una
lista y pasarlas por el AFD.

Regla formal:
    [A-Z][a-z]+[0-9]

    Una letra mayuscula (A-Z), una o mas letras minusculas (a-z) y
    exactamente un digito (0-9) al final.

Las listas de abajo son la lista de trabajo del equipo. Incluyen los casos del
enunciado (seccion 4 de la tabla de trabajo) y una seleccion de etiquetas
generadas con el prompt de PROMPT_USADO. Si se necesita la evidencia literal
del LLM, se corre el prompt y se amplian las listas: la estructura no cambia.

Este archivo no depende de nadie mas: es solo data.
"""

from __future__ import annotations

REGLA = "[A-Z][a-z]+[0-9]"

PROMPT_USADO = """Genera etiquetas cortas para un lenguaje de comandos.
La regla es: una letra mayuscula (A-Z), seguidas de una o mas letras
minusculas (a-z), y exactamente un digito (0-9) al final.
Ejemplos validos: Get1, Run2, Read9.
Devuelve tres grupos:
  - 6 etiquetas validas
  - 8 etiquetas invalidas que violen la regla de forma evidente
  - 10 etiquetas ambiguas que parezcan validas o que enganen a un
    validador ingenuo (por ejemplo, con espacios o digitos de otros
    alfabetos)
Devuelve solo las listas, sin explicaciones."""


def _salida(cadena: str, esperado: bool, categoria: str, nota: str = "") -> dict:
    """Arma un caso de prueba con el formato que consume pruebas_llm.py."""
    return {
        "cadena": cadena,
        "esperado": esperado,
        "categoria": categoria,
        "nota": nota,
    }


# --- Etiquetas que cumplen la regla -----------------------------------------
# Las tres primeras son las del enunciado.
SALIDAS_LLM = [
    _salida("Get1", True, "valida", "Caso base del enunciado"),
    _salida("Run2", True, "valida", "Caso base del enunciado"),
    _salida("Read9", True, "valida", "Caso base del enunciado"),
    _salida("Zz9", True, "valida", "Frontera del alfabeto: Z y z"),
    _salida("Get0", True, "valida", "Frontera del digito: 0 tambien vale"),
    _salida("Ab1", True, "valida", "Etiqueta valida mas corta posible"),

    # --- Etiquetas que violan la regla de forma evidente --------------------
    _salida("get1", False, "invalida", "Empieza con minuscula"),
    _salida("Get", False, "invalida", "Sin digito final"),
    _salida("Get12", False, "invalida", "Dos digitos al final"),
    _salida("Get_1", False, "invalida", "Guion bajo fuera del alfabeto"),
    _salida("G1", False, "invalida", "Falta al menos una minuscula"),
    _salida("GET1", False, "invalida", "Mayusculas donde van minusculas"),
    _salida("", False, "invalida", "Cadena vacia"),
    _salida("Ñandú1", False, "invalida", "Ñ y ú fuera de A-Z / a-z"),

    # --- Etiquetas ambiguas: parecen validas o enganan a un validador ingenuo
    _salida("A1", False, "ambigua", "Parece corta y valida, pero no tiene minusculas"),
    _salida(" Get1", False, "ambigua", "Espacio inicial: un strip() la aceptaria por error"),
    _salida("Get1 ", False, "ambigua", "Espacio final: un strip() la aceptaria por error"),
    _salida("Get 1", False, "ambigua", "Espacio interno entre letras y digito"),
    _salida("Get1x", False, "ambigua", "Caracter despues del digito final"),
    _salida("Get01", False, "ambigua", "Dos digitos: el 0 inicial engana"),
    _salida("Get١", False, "ambigua", "Digito arabe-indico: isdigit() lo acepta, la regla no"),
    _salida("Get1\n", False, "ambigua", "Salto de linea al final"),
    _salida("Get1.", False, "ambigua", "Punto final"),
    _salida("Gét1", False, "ambigua", "e acentuada: isalpha() la acepta, la regla no"),
]

SALIDAS_VALIDAS = [s["cadena"] for s in SALIDAS_LLM if s["categoria"] == "valida"]
SALIDAS_INVALIDAS = [s["cadena"] for s in SALIDAS_LLM if s["categoria"] == "invalida"]
SALIDAS_AMBIGUAS = [s["cadena"] for s in SALIDAS_LLM if s["categoria"] == "ambigua"]


def cadenas() -> list[str]:
    """Devuelve solo las cadenas, en el mismo orden de SALIDAS_LLM."""
    return [salida["cadena"] for salida in SALIDAS_LLM]


# --- Ciclo opcional: el LLM corrige lo que el AFD rechazo -------------------
# A cada cadena rechazada se le pidio al LLM una version corregida.
# Este diccionario es la respuesta del LLM (original -> corregida).
CORRECCIONES_LLM: dict[str, str] = {
    "get1": "Get1",
    "Get": "Get1",
    "Get12": "Get1",
    "Get_1": "Get1",
    "G1": "Ga1",
    "GET1": "Get1",
    "": "Get1",
    "Ñandú1": "Nandu1",
    "A1": "Ab1",
    " Get1": "Get1",
    "Get1 ": "Get1",
    "Get 1": "Get1",
    "Get1x": "Get1",
    "Get01": "Get1",
    "Get١": "Get1",
    "Get1\n": "Get1",
    "Get1.": "Get1",
    "Gét1": "Get1",
}
