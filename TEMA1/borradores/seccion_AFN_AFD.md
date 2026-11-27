# AFN, AFD y equivalencia (Integrante 1)

## 1. Regla formal

Lenguaje reconocido: `[A-Z][a-z]+[0-9]` (ej.: `Get1`, `Run2`, `Read9`).

Alfabeto abstracto Σ = { M, m, d, o }

| Símbolo | Significa |
|---|---|
| M | mayúscula A-Z |
| m | minúscula a-z |
| d | dígito 0-9 |
| o | cualquier otro carácter (`_`, `Ñ`, espacio, etc.) |

## 2. AFN

N = (Q, Σ, δ, q0, F)

- Q = {q0, q1, q2, q3}
- Σ = {M, m, d, o}
- Estado inicial: q0
- Estados finales: F = {q3}

**Bifurcación (no determinismo):** en q1, al leer `m` el autómata puede quedarse en q1 (sigue habiendo minúsculas) **o** pasar a q2 (esta fue la última minúscula). δ(q1, m) = {q1, q2}.

Las transiciones no definidas llevan a ∅ (la rama muere).

| Estado | M | m | d | o |
|---|---|---|---|---|
| → q0 | {q1} | ∅ | ∅ | ∅ |
| q1 | ∅ | **{q1, q2}** | ∅ | ∅ |
| q2 | ∅ | ∅ | {q3} | ∅ |
| *q3 | ∅ | ∅ | ∅ | ∅ |

Diagrama: `diagramas/afn.png` (fuente: `afn.dot`).

## 3. Conversión AFN → AFD (construcción de subconjuntos)

Se parte de {q0} y se calcula, para cada subconjunto alcanzable, el conjunto de estados a los que se llega con cada símbolo.

| Subconjunto | M | m | d | o |
|---|---|---|---|---|
| → {q0} | {q1} | ∅ | ∅ | ∅ |
| {q1} | ∅ | {q1, q2} | ∅ | ∅ |
| {q1, q2} | ∅ | {q1, q2} | {q3} | ∅ |
| *{q3} | ∅ | ∅ | ∅ | ∅ |
| ∅ | ∅ | ∅ | ∅ | ∅ |

Pasos clave:

1. {q0} --M--> {q1}.
2. {q1} --m--> {q1} ∪ {q2} = {q1, q2}.
3. {q1, q2} --m--> {q1, q2} (q1 aporta {q1, q2}, q2 no aporta nada con `m`).
4. {q1, q2} --d--> {q3} (solo q2 tiene transición con `d`).
5. Es final todo subconjunto que contenga q3, es decir, {q3}.
6. ∅ es el estado de rechazo (trampa).

Resultado: **5 estados** (4 útiles + trampa).

## 4. AFD resultante y su correspondencia con `validadorADF.py`

| Subconjunto | Estado en el código | M | m | d | o |
|---|---|---|---|---|---|
| → {q0} | q0 | q1 | q_err | q_err | q_err |
| {q1} | q1 | q_err | q2 | q_err | q_err |
| {q1, q2} | q2 | q_err | q2 | q3 | q_err |
| *{q3} | q3 | q_err | q_err | q_err | q_err |
| ∅ | q_err | q_err | q_err | q_err | q_err |

Esta tabla coincide celda por celda con `TRANSICIONES` de `validadorADF.py`. El estado `q2` del código corresponde al subconjunto {q1, q2} del AFN.

Diagrama: `diagramas/afd.png` (fuente: `afd.dot`).

## 5. Equivalencia

- **Por construcción:** la construcción de subconjuntos produce siempre un AFD que reconoce el mismo lenguaje que el AFN (Hopcroft, Motwani y Ullman, 2007).
- **Por comprobación:** `verificar_equivalencia.py` compara el AFN, el AFD del código y la expresión regular sobre las 21 845 cadenas de longitud 0 a 7 sobre {M, m, d, o}. Resultado: 0 discrepancias.

Para correrlo: `python verificar_equivalencia.py`

## Referencia (APA 7)

Hopcroft, J. E., Motwani, R., & Ullman, J. D. (2007). *Introduction to automata theory, languages, and computation* (3.ª ed.). Pearson.
