import os
import re
import time
from fractions import Fraction

ARCHIVO_SALIDA = "resultado_python.txt"   # distinto del de C para no sobrescribirlo
ARCHIVO_C = "resultado_c.txt"             # solo se LEE, para comparar tiempos
REPETICIONES = 1000


def generar_fila(n):
    """Fila n del triangulo de Pascal usando una sola lista (programacion dinamica).
    Los enteros de Python no tienen limite, asi que es exacto para cualquier n."""
    fila = [0] * (n + 1)
    fila[0] = 1
    for i in range(1, n + 1):
        fila[i] = 1                        # el ultimo de cada fila es 1
        for j in range(i - 1, 0, -1):      # de derecha a izquierda para no pisar valores
            fila[j] = fila[j] + fila[j - 1]
    return fila


def polinomio_a_texto(f, n, sep):
    """Arma '(x+1)^n = x^n+...+1' (el coeficiente 1 no se escribe, salvo el independiente)."""
    partes = []
    for k in range(n + 1):
        p = n - k
        t = ""
        if f[k] != 1 or p == 0:
            t += str(f[k])
        if p == 1:
            t += "x"
        elif p > 1:
            t += f"x^{p}"
        partes.append(t)
    return f"(x+1)^{n} = " + sep.join(partes)


def imprimir_triangulo(archivo, n):
    archivo.write("Triangulo de Pascal y polinomios:\n\n")
    for i in range(n + 1):
        f = generar_fila(i)
        izquierda = polinomio_a_texto(f, i, "+")
        derecha = " " * ((n - i) * 2) + "".join(f"{c:4d}" for c in f)
        archivo.write(f"{izquierda:<76}(n ={i}): {derecha}\n")
    archivo.write("\n")


def fmt(v):
    """Entero exacto si lo es; si no, decimal."""
    return str(v.numerator) if v.denominator == 1 else f"{float(v):.10g}"


def leer_tiempo_c():
    """Si existe resultado_c.txt, extrae el tiempo de C para comparar."""
    if not os.path.exists(ARCHIVO_C):
        return None
    with open(ARCHIVO_C, encoding="utf-8", errors="ignore") as f:
        m = re.search(r"Tiempo C n=100:\s*([0-9.]+)", f.read())
    return float(m.group(1)) if m else None


def main():
    try:
        n = int(input("Ingrese n (entero >= 0): "))
        if n < 0:
            raise ValueError
    except ValueError:
        print("n invalido: debe ser un entero no negativo")
        return
    try:
        x = Fraction(input("Ingrese x: ").strip().replace(",", "."))
    except (ValueError, ZeroDivisionError):
        print("x invalido")
        return

    with open(ARCHIVO_SALIDA, "w", encoding="utf-8") as out:
        imprimir_triangulo(out, min(n, 10))

        #Coeficientes y polinomio
        coef = generar_fila(n)
        out.write("Coeficientes: " + " ".join(map(str, coef)) + " \n\n")
        out.write("Polinomio: " + polinomio_a_texto(coef, n, " + ") + "\n")

        #Calculo por pasos de f(x). Con Fraction el resultado es exacto
        out.write(f"\nCalculo por pasos con x = {fmt(x)}:\n")
        total = Fraction(0)
        for k in range(n + 1):
            p = n - k
            valor = coef[k] * x ** p
            total += valor
            out.write(f"Paso {k + 1}: {coef[k]} * ({fmt(x)})^{p} = {fmt(valor)}\n")
        out.write(f"\nf({fmt(x)}) = {fmt(total)}\n")
        out.write(f"Verificacion (x+1)^n = {fmt((x + 1) ** n)}\n")

        #Medicion de tiempo para n = 100,1000 repeticiones, promedio
        inicio = time.perf_counter()
        for _ in range(REPETICIONES):
            generar_fila(100)
        promedio = (time.perf_counter() - inicio) / REPETICIONES
        out.write(f"\nTiempo Python n=100: {promedio:.9f} segundos "
                  f"(promedio de {REPETICIONES} corridas)\n")

        #Comparacion con C,si se ejecuto antes el programa en C
        t_c = leer_tiempo_c()
        if t_c is not None and t_c > 0:
            out.write(f"Tiempo C      n=100: {t_c:.9f} segundos\n")
            out.write(f"Python es {promedio / t_c:.1f} veces mas lento que C\n")
        else:
            out.write("Ejecute primero el programa en C para comparar tiempos.\n")

    print(f"Listo, revisa el archivo {ARCHIVO_SALIDA}")


if __name__ == "__main__":
    main()
