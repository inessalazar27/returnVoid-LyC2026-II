# TEMA 1 · Planteamiento 10: AFD y AFN

Equipo **return void;** · Lenguajes y Compiladores · UNEG 2026-II

**Eslogan:** `// nada que devolver, todo por compilar`

## Integrantes

- Ines Salazar, C.I. 31.701.669, Sección 1
- Licett Avendaño, C.I. 18.886.013, Sección 1
- Lismarx Gamboa, C.I. 30.994.146, Sección 1
- Fabiola Olaya, C.I. 31.869.294, Sección 3

## Entregables

| Entregable | Ubicación |
|---|---|
| Informe de investigación (APA) | `Informe-AFDyAFNTema1final.pdf` |
| Presentación de apoyo | `Autómatas Finitos AFD y AFN -materialVideo.pdf` |
| Video de defensa | videoTema1.md |
| Código del reto evaluado | Archivos `.py` de esta carpeta |

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
| `resultados_llm.json`, `reporte_limites.md` | Reportes generados por los scripts. |
| `borradores/` | Textos de trabajo del equipo (no son entregables). |

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
