# Dependencias de los ejercicios

Esta carpeta contiene ejercicios en **Python, C, C++ y JavaScript**. Ninguno usa
librerías externas: todo el código trabaja con la biblioteca estándar de cada
lenguaje. Por eso **no hace falta ejecutar `pip install` ni `npm install`**.

## Resumen

| Ejercicio | Archivo | Lenguaje | Requisito mínimo |
|---|---|---|---|
| 1 | `ejercicio1/ejercicio1_caballo.js` | JavaScript | Node.js 18 o superior |
| 1 | `ejercicio1/ejercicio1_caballo_c.cpp` | C++ | g++ con C++17 |
| 2 | `ejercicio2/triangulo_pascal.py` | Python | Python 3.12 o superior |
| 2 | `ejercicio2/triangulo_pascal.c` | C | gcc con C11 |
| 3 | `ejercicio3/palabras_reservadas.c` | C | gcc con C11 |

> **Nota sobre Python:** el ejercicio 2 usa comillas dobles dentro de un
> *f-string*, una sintaxis válida desde Python 3.12. Con versiones anteriores
> el archivo produce un `SyntaxError`.

## Instalación de las herramientas

### Windows

Con `winget` (PowerShell):

```powershell
winget install --id Python.Python.3.12 -e
winget install --id OpenJS.NodeJS.LTS -e
winget install --id MSYS2.MSYS2 -e
```

Después, en una terminal de MSYS2 (UCRT64), instalar el compilador:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc
```

Agregar `C:\msys64\ucrt64\bin` al `PATH` para tener `gcc` y `g++` disponibles.
También sirve cualquier instalación de MinGW-w64, por ejemplo la que incluye
Code::Blocks.

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y python3 nodejs npm build-essential
```

### macOS

```bash
brew install python node gcc
```

## Verificar que todo está instalado

```bash
python3 --version
node --version
gcc --version
g++ --version
```

Versiones con las que se probaron los ejercicios:

- Python 3.14.6
- Node.js 24.16.0
- gcc / g++ 14.2.0 (MinGW-w64)

Cualquier versión igual o superior a la indicada en la tabla debería funcionar.

## Compilar y ejecutar

### Ejercicio 1 — JavaScript

```bash
node ejercicio1/ejercicio1_caballo.js
```

### Ejercicio 1 — C++

```bash
g++ -std=c++17 -O2 ejercicio1/ejercicio1_caballo_c.cpp -o ejercicio1/ejercicio1_caballo_c.exe
./ejercicio1/ejercicio1_caballo_c.exe
```

### Ejercicio 2 — Python

```bash
python ejercicio2/triangulo_pascal.py
```

### Ejercicio 2 — C

```bash
gcc -std=c11 -O2 ejercicio2/triangulo_pascal.c -o ejercicio2/triangulo_pascal.exe
./ejercicio2/triangulo_pascal.exe
```

### Ejercicio 3 — C

```bash
gcc -std=c11 -O2 ejercicio3/palabras_reservadas.c -o ejercicio3/palabras_reservadas.exe
./ejercicio3/palabras_reservadas.exe ruta/al/archivo.c
```

Si no se pasa la ruta como argumento, el programa la pide por teclado. El
resultado se guarda en `traducido.txt`, en el directorio desde donde se ejecuta
el programa.
