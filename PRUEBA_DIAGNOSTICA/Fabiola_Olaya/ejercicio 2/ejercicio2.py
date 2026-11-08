import sys
import time
from contextlib import redirect_stdout
from datetime import datetime


class SalidaDuplicada:
    def __init__(self, *salidas):
        self.salidas = salidas

    def write(self, texto):
        for salida in self.salidas:
            salida.write(texto)
        return len(texto)

    def flush(self):
        for salida in self.salidas:
            salida.flush()


def generar_pascal_dinamico(n, mostrar_triangulo=False):
    # Reserva dinamica en el Heap para la fila inicial (n = 0)
    fila_actual = [1]

    if mostrar_triangulo:
        print(f"\n--- Triangulo de Pascal hasta n = {n} (Memoria Dinamica) ---")
        print("(n = 0): 1")

    for i in range(1, n + 1):
        # Crear nueva lista en memoria dinamica para la fila i
        nueva_fila = [1] * (i + 1)
        for j in range(1, i):
            nueva_fila[j] = fila_actual[j - 1] + fila_actual[j]

        if mostrar_triangulo:
            fila_str = " ".join(str(val) for val in nueva_fila)
            print(f"(n = {i}): {fila_str}")

        # Actualizar referencia (el recolector de basura libera la fila anterior)
        fila_actual = nueva_fila

    return fila_actual


def formatear_polinomio(n, coefs):
    if n == 0:
        return "1"

    terminos = []
    for i, c in enumerate(coefs):
        exp = n - i
        if exp == 0:
            terminos.append(f"{c}")
        elif exp == 1:
            terminos.append("x" if c == 1 else f"{c}x")
        else:
            terminos.append(f"x^{exp}" if c == 1 else f"{c}x^{exp}")

    return " + ".join(terminos)


def calcular_paso_a_paso(n, x, coefs):
    print(f"\n--- b) Calculo paso a paso para x = {x} en f(x) = (x+1)^{n} ---")

    sustitucion = []
    potencias_str = []
    productos = []

    for i, c in enumerate(coefs):
        exp = n - i
        pot = x ** exp
        prod = c * pot

        if exp == 0:
            sustitucion.append(f"{c}")
            potencias_str.append(f"{c}")
        elif exp == 1:
            sustitucion.append(f"{c}*({x})")
            potencias_str.append(f"{c}*({pot})")
        else:
            sustitucion.append(f"{c}*({x}^{exp})")
            potencias_str.append(f"{c}*({pot})")

        productos.append(prod)

    print(f"Paso 1 (Sustitucion):  f({x}) = " + " + ".join(sustitucion))
    print(f"Paso 2 (Potencias):    f({x}) = " + " + ".join(potencias_str))
    print(f"Paso 3 (Productos):    f({x}) = " + " + ".join(str(p) for p in productos))

    resultado_final = sum(productos)
    print(f"Paso 4 (Resultado):    f({x}) = {resultado_final}")


def main():
    print("====================================================")
    print("   EJERCICIO 2 - TRIANGULO DE PASCAL EN PYTHON      ")
    print("====================================================")

    try:
        entrada_n = input("Ingrese un numero entero no negativo n (ej. 4 o 5): ").strip()
        n = int(entrada_n) if entrada_n else 5
        if n < 0:
            n = 5
    except ValueError:
        n = 5

    try:
        entrada_x = input(f"Ingrese el valor de x para evaluar f(x) = (x+1)^{n} (ej. 2): ").strip()
        x = float(entrada_x) if "." in entrada_x else (int(entrada_x) if entrada_x else 2)
    except ValueError:
        x = 2

    # 1. Parte a) y b) con el n y x ingresados
    coefs = generar_pascal_dinamico(n, mostrar_triangulo=True)
    polinomio_str = formatear_polinomio(n, coefs)

    print(f"\n--- a) Resultado del Polinomio (x+1)^{n} ---")
    print(f"(x+1)^{n} = {polinomio_str}")

    calcular_paso_a_paso(n, x, coefs)

    # 2. Medicion de tiempo para n = 100
    n_benchmark = 100
    inicio = time.perf_counter()
    coefs_100 = generar_pascal_dinamico(n_benchmark, mostrar_triangulo=False)
    fin = time.perf_counter()
    tiempo_segundos = fin - inicio

    print("\n====================================================")
    print("   MEDICION DE TIEMPO PARA n = 100 (PYTHON)         ")
    print("====================================================")
    print(f"Polinomio generado (x+1)^100: x^100 + {coefs_100[1]}x^99 + {coefs_100[2]}x^98 + ... + {coefs_100[99]}x + 1")
    print(f"Tiempo en generar n = 100 en Python: {tiempo_segundos:.8f} segundos ({tiempo_segundos * 1e6:.3f} microsegundos)")


if __name__ == "__main__":
    with open("resultado_ejercicio2_python.txt", "a", encoding="utf-8") as archivo:
        archivo.write(f"\n=== Ejecucion: {datetime.now():%Y-%m-%d %H:%M:%S} ===\n")
        with redirect_stdout(SalidaDuplicada(sys.stdout, archivo)):
            main()