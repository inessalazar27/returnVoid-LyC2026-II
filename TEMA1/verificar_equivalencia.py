"""
Verificación de equivalencia AFN <-> AFD para la regla [A-Z][a-z]+[0-9].
Compara, para TODAS las cadenas hasta longitud 7 sobre {M, m, d, o}:
  1) el AFN simulado con conjuntos de estados,
  2) el AFD de validadorADF.py (tabla TRANSICIONES copiada tal cual),
  3) la expresión regular de referencia.
Uso: python verificar_equivalencia.py
"""
from itertools import product
import re
import sys
sys.stdout.reconfigure(encoding="utf-8")

# --- AFN (Integrante 1): sin estado de rechazo; lo no definido = conjunto vacío
AFN = {
    ("q0", "M"): {"q1"},
    ("q1", "m"): {"q1", "q2"},   # bifurcación
    ("q2", "d"): {"q3"},
}
AFN_FINALES = {"q3"}

def acepta_afn(cadena):
    estados = {"q0"}
    for s in cadena:
        estados = set().union(*(AFN.get((q, s), set()) for q in estados)) if estados else set()
    return bool(estados & AFN_FINALES)

# --- AFD (validadorADF.py)
T = {}
for e, fila in {
    "q0": ("q1", "q_err", "q_err", "q_err"),
    "q1": ("q_err", "q2", "q_err", "q_err"),
    "q2": ("q_err", "q2", "q3", "q_err"),
    "q3": ("q_err",) * 4,
    "q_err": ("q_err",) * 4,
}.items():
    for s, d in zip("Mmdo", fila):
        T[(e, s)] = d

def acepta_afd(cadena):
    e = "q0"
    for s in cadena:
        e = T[(e, s)]
    return e == "q3"

# --- Subconjuntos: reconstruye el AFD desde el AFN y lo compara con la tabla
def subconjuntos():
    inicio = frozenset({"q0"})
    tabla, pend, vistos = {}, [inicio], {inicio}
    while pend:
        S = pend.pop()
        for s in "Mmdo":
            N = frozenset().union(*(AFN.get((q, s), set()) for q in S))
            tabla[(S, s)] = N
            if N not in vistos:
                vistos.add(N); pend.append(N)
    return tabla, vistos

# --- Referencia: M->A-Z, m->a-z, d->0-9, o->'_'
REGEX = re.compile(r"[A-Z][a-z]+[0-9]")
REAL = {"M": "G", "m": "e", "d": "1", "o": "_"}

if __name__ == "__main__":
    tabla, estados = subconjuntos()
    print(f"Estados del AFD por subconjuntos (alcanzables): {len(estados)}")
    for S in sorted(estados, key=lambda x: (len(x), sorted(x))):
        print("  ", set(S) if S else "∅")

    total = fallos = 0
    for n in range(0, 8):
        for tupla in product("Mmdo", repeat=n):
            c = "".join(tupla)
            real = "".join(REAL[s] for s in c)
            a, b, r = acepta_afn(c), acepta_afd(c), bool(REGEX.fullmatch(real))
            total += 1
            if not (a == b == r):
                fallos += 1
                print("DISCREPANCIA:", repr(c), a, b, r)
    print(f"\nCadenas probadas: {total}  |  Discrepancias: {fallos}")
    print("RESULTADO:", "AFN, AFD y regex son equivalentes" if fallos == 0 else "NO equivalentes")
