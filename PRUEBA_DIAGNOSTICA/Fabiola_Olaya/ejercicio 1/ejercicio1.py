import time

def verificar_movimientos_caballo(ak, n, cadena_c):
    columnas = {'a': 1, 'b': 2, 'c': 3, 'd': 4, 'e': 5, 'f': 6, 'g': 7, 'h': 8}
    filas = {'1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8}

    # Validar posicion inicial Ak
    if len(ak) != 2 or ak[0] not in columnas or ak[1] not in filas:
        return "INCORRECTO: Posicion inicial invalida."

    col_actual = columnas[ak[0]]
    fila_actual = filas[ak[1]]

    movimientos = cadena_c.strip().split()
    limite = min(n, len(movimientos))

    for i in range(limite):
        mov = movimientos[i]
        num_mov = i + 1

        # Validar formato de 3 caracteres (Ej: Kc2)
        if len(mov) != 3:
            return f"movimiento INCORRECTO falla en el movimiento {num_mov}."

        pieza, col_dest_char, fila_dest_char = mov[0], mov[1], mov[2]

        # Verificar que la pieza sea 'K' (Caballo en notacion inglesa)
        if pieza != 'K':
            return f"movimiento INCORRECTO falla letra {pieza} no pertenece a un caballo."

        # Verificar que este dentro del tablero 8x8
        if col_dest_char not in columnas or fila_dest_char not in filas:
            return f"movimiento INCORRECTO falla en el movimiento {num_mov}."

        col_dest = columnas[col_dest_char]
        fila_dest = filas[fila_dest_char]

        # Calcular desplazamiento en 'L': (1, 2) o (2, 1)
        diff_col = abs(col_dest - col_actual)
        diff_fila = abs(fila_dest - fila_actual)

        if not ((diff_col == 1 and diff_fila == 2) or (diff_col == 2 and diff_fila == 1)):
            return f"INCORRECTO falla en el movimiento {num_mov}."

        # Actualizar posicion actual
        col_actual = col_dest
        fila_actual = fila_dest

    return "movimiento CORRECTO."


if __name__ == "__main__":
    lineas_salida = ["=== CASOS DE PRUEBA DEL ENUNCIADO (PYTHON) ==="]

    ejemplos = [
        ("a1", 4, "Kc2 Kd4 Kb3 Kc5"),
        ("a1", 4, "Kc2 Kd4 Kb4 Kc5"),
        ("a2", 5, "Kc3 Ke4 Kf6 Ke8 Kg7"),
        ("a2", 5, "Kc8 Ke4 Kf6 Ke8 Kg7"),
        ("a8", 2, "Kb6 Kd7"),
        ("a8", 2, "Kb6 Kb7"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4")
    ]

    for ak, n, c in ejemplos:
        res = verificar_movimientos_caballo(ak, n, c)
        texto = f"Para Ak={ak} n={n} y C={c} salida: {res}"
        print(texto)
        lineas_salida.append(texto)

    # Los 500 movimientos exactos del PDF actualizado (Paginas 1 y 2)
    cadena_500 = (
        "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
        "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
        "Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
        "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
        "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
        "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 "
        "Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 "
        "Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 "
        "Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 "
        "Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
        "Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 "
        "Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
        "Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
        "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
        "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
        "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 "
        "Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
        "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 "
        "Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 "
        "Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 "
        "Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 "
        "Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4"
    )

    ak_inicial = "a2"
    n_movs = 500

    # Medir tiempo en verificar 10 veces los 500 movimientos
    inicio = time.perf_counter()
    for _ in range(10):
        res_500 = verificar_movimientos_caballo(ak_inicial, n_movs, cadena_500)
    fin = time.perf_counter()
    tiempo_500 = fin - inicio

    lineas_salida.append("\n=== MEDICION DE TIEMPO (10 VERIFICACIONES DE 500 MOVIMIENTOS) ===")
    lineas_salida.append(f"Posicion inicial: {ak_inicial}")
    lineas_salida.append(f"Total de tokens en la cadena: {len(cadena_500.split())}")
    lineas_salida.append(f"Salida obtenida: {res_500}")
    lineas_salida.append(f"Tiempo en verificar 10 veces los 500 movimientos (Python): {tiempo_500:.8f} segundos")

    print("\n" + "\n".join(lineas_salida[-5:]))

    with open("resultado_ejercicio1.txt", "w", encoding="utf-8") as archivo:
        archivo.write("\n".join(lineas_salida))