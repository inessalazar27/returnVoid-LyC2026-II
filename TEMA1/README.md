# TEMA 1 · Planteamiento 10: AFD y AFN

Equipo **return void;** · Lenguajes y Compiladores · UNEG 2026-II

## Reto evaluado

Validador de etiquetas generadas por un LLM mediante un AFD en Python.
Regla formal: `[A-Z][a-z]+[0-9]` (válidas: `Get1`, `Run2`, `Read9`).

## Archivos

| Archivo | Qué hace |
|---|---|
| `validadorADF.py` | Motor del AFD. Función `validarEtiqueta(cadena)`. |
| `salidas_llm.py` | Prompt, lista de etiquetas del LLM y sus correcciones. |
| `pruebas_llm.py` | Pasa las etiquetas por el AFD y reporta resultados. |
| `test_limites.py` | 20 casos límite contra el motor real. |
| `verificar_equivalencia.py` | Comprueba que AFN, AFD y regex son equivalentes. |
| `diagramas/` | Diagramas del AFN y del AFD (`.png` y fuente `.dot`). |
| `seccion_AFN_AFD.md` | Tablas de transición y conversión AFN → AFD. |
| `seccion_RETO_EVALUADO.md` | Sección del informe sobre el reto. |

## Cómo correrlo

Requiere Python 3.9 o superior. Desde esta carpeta:

```bash
python validadorADF.py                       # modo interactivo (5 etiquetas)
python pruebas_llm.py                        # etiquetas del LLM + correcciones
python pruebas_llm.py --verbose              # con detalle de rechazos
python pruebas_llm.py --guardar resultados_llm.json   # guarda el reporte JSON
python test_limites.py --guardar             # casos límite + reporte_limites.md
python verificar_equivalencia.py             # AFN vs AFD vs regex
```

Los diagramas se regeneran con Graphviz: `dot -Tpng diagramas/afd.dot -o diagramas/afd.png` (igual con `afn`).
