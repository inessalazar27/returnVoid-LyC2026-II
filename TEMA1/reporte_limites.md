| Cadena | Esperado | Obtenido | Estado final | Resultado | Caso |
|---|---|---|---|---|---|
| Get1 | acepta | acepta | q3 | OK | caso base |
| Run2 | acepta | acepta | q3 | OK | caso base |
| Read9 | acepta | acepta | q3 | OK | caso base |
| Ab1 | acepta | acepta | q3 | OK | mínimo válido: 1 minúscula |
| Get0 | acepta | acepta | q3 | OK | dígito 0 |
| Gaaaaaaaaaaaaaaaa1 | acepta | acepta | q3 | OK | muchas minúsculas |
| get1 | rechaza | rechaza | q_err | OK | empieza con minúscula |
| Get | rechaza | rechaza | q2 | OK | sin dígito final |
| Get12 | rechaza | rechaza | q_err | OK | dos dígitos |
| Get_1 | rechaza | rechaza | q_err | OK | guion bajo fuera del alfabeto |
| G1 | rechaza | rechaza | q_err | OK | falta minúscula |
| A1 | rechaza | rechaza | q_err | OK | falta minúscula |
| GET1 | rechaza | rechaza | q_err | OK | mayúsculas donde van minúsculas |
| '' | rechaza | rechaza | q0 | OK | cadena vacía |
| Ñandú1 | rechaza | rechaza | q_err | OK | Ñ fuera de A-Z |
| Gét1 | rechaza | rechaza | q_err | OK | tilde fuera de a-z |
| Ge1t | rechaza | rechaza | q_err | OK | dígito en medio |
| 'Get1 ' | rechaza | rechaza | q_err | OK | espacio final |
| 'Get1\n' | rechaza | rechaza | q_err | OK | salto de línea final |
| Get٣ | rechaza | rechaza | q_err | OK | dígito arábigo-índico (U+0663) |

**20/20 casos coinciden.**
