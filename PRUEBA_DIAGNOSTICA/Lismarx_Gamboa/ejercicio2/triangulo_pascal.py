"""
Dado un número entero no negativo n, a) genere los coeficientes del polinomio (x+1) n, muestre el resultado
del polinomio y b) muestre por pasos el cálculo para x dado, f(x) = (x+1)n ,según el polinomio generado.
Implemente el algoritmo utilizando memoria dinámica.
Para generar los polinomios de (x+1)n utilice el triangulo de pascal.
"""
import time


def triangulo_pascal(num, x):
    if num >= 0:
        index = 0
        # el triangulo de pascal inicia con un termino y va incrementandose en uno en cada linea
        cantidad_terminos = 1
        # se guarda la linea anterior para poder usar la suma de los componentes de su array en la actual
        linea_anterior = []
        resultados = 0
        print(f"exponente {num}")
        for n in range(num + 1):
            linea_actual = []
            exponente = cantidad_terminos - 1
            exp_iterar = cantidad_terminos - 1
            if not linea_anterior:
                linea_actual = [1]  # si no hay linea anterior es solo 1
            else:
                index = 0
                for t in range(cantidad_terminos):
                    termino = []
                    if index == 0:
                        termino = linea_anterior[index]
                        # el primer valor siempre es 1
                        linea_actual.append(termino)
                    elif index == cantidad_terminos - 1:
                        # el ultimo valor siempre es 1
                        termino = linea_anterior[-1]
                        linea_actual.append(termino)
                    else:
                        termino = linea_anterior[index -
                                                 1] + linea_anterior[index]
                        # los terminos del medio son la suma de los dos terminos arriba de este
                        linea_actual.append(termino)
                    index += 1

            cantidad_terminos += 1
            linea_anterior = linea_actual

            for t in linea_actual:
                if exp_iterar > 0:
                    print(f"{t if t > 1 else ""}x^{exp_iterar}", end=" + ")
                    exp_iterar -= 1
                else:
                    print(f"{t}")

            if (len(linea_actual) == num + 1) or (num == 0):
                print(f"Evaluar {linea_actual}")
                for t in linea_actual:
                    termino_resultado = []
                    if exponente > 0:
                        termino_resultado = t * (x)**exponente
                        print(
                            f"operacion {t}*{x}^{exponente} = {termino_resultado}")
                        resultados += termino_resultado
                        exponente -= 1
                    else:
                        termino_resultado = t
                        print(f"operacion {t} = {termino_resultado}")
                        resultados += termino_resultado
                print(f"Resultado Final: {resultados}")

    else:
        print("El numero no puede ser menor a 0")


def main():
    inicio = time.perf_counter()
    seguir = True
    while seguir:
        try:
            str_exponente = input(
                "Ingrese el numero del exponente para evaluar el triangulo de pascal: ")
            exponente = int(str_exponente)
            print(exponente)
        except ValueError:
            print("Error, numero no valido")
            continue

        try:
            str_valor_x = input(
                "Ingrese el valor de x para evaluar el triangulo de pascal: ")
            valor_x = int(str_valor_x)
            print(valor_x)
        except ValueError:
            print("Error, numero no valido")
            continue

        triangulo_pascal(exponente, valor_x)
        seguir = False
    fin = time.perf_counter()
    print(f"Tiempo de ejecucion del programa: {fin - inicio}s")


main()
