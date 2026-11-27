# RETO EVALUADO: validador de etiquetas generadas por un LLM mediante un AFD

## En qué consistió el reto

El equipo *return void;* se planteó el siguiente reto: un modelo de lenguaje (LLM) genera etiquetas o comandos cortos, y un autómata finito determinista (AFD) implementado en Python decide si cada salida cumple una regla formal y, cuando la rechaza, explica por qué. De este modo, la garantía de corrección la da el autómata y no el LLM.

La regla formal es la expresión regular `[A-Z][a-z]+[0-9]`: una letra mayúscula, seguida de una o más minúsculas, y exactamente un dígito final (ejemplos válidos: `Get1`, `Run2`, `Read9`). El reto conecta con el planteamiento del informe (AFD y AFN) porque el análisis léxico de un compilador cumple la misma función: filtrar secuencias de caracteres antes de que otra fase las procese (Aho et al., 2006).

## Cómo se resolvió

**1. Regla formal y autómatas.** Se definió el alfabeto abstracto Σ = {M, m, d, o} (mayúscula, minúscula, dígito y cualquier otro carácter). Se construyó un AFN con una bifurcación en q1 (con `m` va a q1 y a q2) y, mediante la construcción de subconjuntos, se obtuvo un AFD de cinco estados (cuatro útiles y un estado de rechazo). Las tablas y los diagramas están en la sección de AFN y AFD de este informe. Un programa de verificación comparó el AFN, el AFD y la expresión regular sobre las 21 845 cadenas de longitud 0 a 7 sobre Σ, sin encontrar discrepancias.

**2. Motor del AFD.** El archivo `validadorADF.py` implementa el AFD con los estados q0 (inicio), q1 (leyó la mayúscula), q2 (leyó una o más minúsculas), q3 (estado final, leyó el dígito) y q_err (estado de rechazo). La función `validarEtiqueta(cadena)` lee la cadena símbolo a símbolo y devuelve si es válida, el estado final y el motivo del rechazo, indicando el estado, el carácter y la posición donde falló.

**3. Salidas del LLM y script de pruebas.** Se le pidió a un LLM una lista de etiquetas válidas, inválidas y ambiguas (`salidas_llm.py`), y `pruebas_llm.py` las pasa una por una por el AFD. Opcionalmente, toma las correcciones propuestas para las etiquetas rechazadas y las vuelve a validar.

**4. Casos límite.** `test_limites.py` ejecuta 20 casos límite contra el motor real (espacios, saltos de línea, tildes, `Ñ`, dígitos de otros alfabetos, cadena vacía, cadenas largas, entre otros).

## Resultados

| Prueba | Resultado |
|---|---|
| Etiquetas del LLM procesadas | 24 |
| Aceptadas por el AFD | 6 |
| Rechazadas por el AFD | 18 |
| Coinciden con lo esperado | 24/24 |
| Correcciones propuestas por el LLM aceptadas al revalidar | 18/18 |
| Casos límite (`test_limites.py`) | 20/20 |

Por categoría, las 6 etiquetas válidas fueron aceptadas y las 8 inválidas y 10 ambiguas fueron rechazadas.

**Errores del LLM que el AFD detectó:** inicio con minúscula (`get1`), ausencia de dígito final (`Get`), dos dígitos finales (`Get12`, `Get01`), caracteres fuera del alfabeto (`Get_1`), falta de minúscula (`G1`, `A1`), mayúsculas donde van minúsculas (`GET1`), cadena vacía, letras no ASCII (`Ñandú1`, `Gét1`), espacios (`" Get1"`, `"Get1 "`, `"Get 1"`), salto de línea final, punto final (`Get1.`), carácter extra tras el dígito (`Get1x`) y dígito de otro alfabeto (`Get١`).

El resultado más relevante es que el AFD rechaza correctamente los casos "ambiguos", que a simple vista pueden parecer válidos (espacios invisibles, saltos de línea, dígitos de otros sistemas de numeración). Esto se debe a que el motor compara rangos ASCII explícitos y no funciones como `isdigit()`, que aceptan dígitos Unicode.

## Limitaciones

- La lista de etiquetas es la lista de trabajo del equipo: incluye los casos del enunciado y una selección de etiquetas generadas con el prompt; no es una transcripción cruda de la respuesta del LLM.
- Las correcciones "del LLM" están almacenadas en un diccionario dentro de `salidas_llm.py`; no se hace una llamada en vivo al modelo.
- El validador solo reconoce el formato de la etiqueta; no verifica que la palabra tenga sentido (por ejemplo, `Zzz9` es aceptada).

## Cómo reproducirlo

Ver `README.md` en la carpeta TEMA1: `python pruebas_llm.py`, `python test_limites.py` y `python verificar_equivalencia.py`.

## Referencias (APA 7)

Aho, A. V., Lam, M. S., Sethi, R., & Ullman, J. D. (2006). *Compilers: Principles, techniques, and tools* (2.ª ed.). Pearson.

Hopcroft, J. E., Motwani, R., & Ullman, J. D. (2007). *Introduction to automata theory, languages, and computation* (3.ª ed.). Pearson.
