"""Ejercicio 1: validador de movimientos de caballo (version Python).
Metodo: columnas movidas x filas movidas = 2  ->  movimiento de caballo
"""
import time


def revisar(mov, num, col, fila):
    """Revisa UN movimiento.
    Devuelve (correcto, mensaje, nueva_col, nueva_fila)."""
    letra = mov[0]
    if letra == 'k':                      # acepta k minuscula
        letra = 'K'

    if letra != 'K':
        return (False,
                f"movimiento INCORRECTO falla letra {mov[0]} no pertenece a un caballo.",
                col, fila)

    if len(mov) != 3:
        return False, f"INCORRECTO falla en el movimiento {num}.", col, fila

    cc = mov[1]
    if 'A' <= cc <= 'H':                  # acepta columna en mayuscula
        cc = cc.lower()

    if not ('a' <= cc <= 'h') or not ('1' <= mov[2] <= '8'):
        return False, f"INCORRECTO falla en el movimiento {num}.", col, fila

    c = ord(cc) - ord('a') + 1
    f = int(mov[2])

    if abs(c - col) * abs(f - fila) != 2:
        return False, f"INCORRECTO falla en el movimiento {num}.", col, fila

    return True, "", c, f


def validar(inicio, cadena):
    """Revisa una cadena de movimientos. Se detiene en el primer error.
    Devuelve (ok, mensaje)."""
    col = ord(inicio[0]) - ord('a') + 1
    fila = int(inicio[1])

    num = 0
    for tok in cadena.split():
        num += 1
        ok, msg, col, fila = revisar(tok, num, col, fila)
        if not ok:
            return False, msg
    return True, "movimiento CORRECTO."


def contar_movimientos(cadena):
    """Cuenta cuantos movimientos hay en una cadena."""
    return len(cadena.split())


def leer_posicion():
    """Pide la posicion inicial hasta que sea valida."""
    while True:
        try:
            buf = input("Posicion inicial (ej. a2): ").strip()
        except EOFError:
            return "a1"
        buf = buf.lower()
        if len(buf) == 2 and 'a' <= buf[0] <= 'h' and '1' <= buf[1] <= '8':
            return buf
        print("Posicion invalida. Use columna a-h y fila 1-8.")


def opcion_verificar():
    """Opcion 1: el usuario escribe posicion, n y movimientos; se verifica."""
    print("\n--- VERIFICAR MOVIMIENTOS ---")
    inicio = leer_posicion()

    while True:
        try:
            n = int(input("Cantidad de movimientos n: "))
        except ValueError:
            n = 0
        except EOFError:
            return
        if n > 0:
            break
        print("n debe ser un numero mayor que 0.")

    while True:
        try:
            entrada = input(f"Escriba los {n} movimientos (ej. Kc2 Kd4 Kb3): ").strip()
        except EOFError:
            return
        real = contar_movimientos(entrada)
        if real == n:
            break
        print(f"Indico n={n} pero escribio {real} movimientos. Intente de nuevo.")

    _, msg = validar(inicio, entrada)
    print(f"\nPara Ak = {inicio} n= {n} y C= {entrada}")
    print(f"salida: {msg}")


def opcion_medir():
    """Opcion 2: medir el tiempo con movimientos.txt."""
    print("\n--- MEDIR TIEMPO (movimientos.txt) ---")
    inicio = leer_posicion()

    reps = 10
    try:
        r = int(input("Cuantas veces verificar? (Enter = 10): "))
        if r > 0:
            reps = r
    except (ValueError, EOFError):
        pass

    # utf-8-sig ignora el BOM si el archivo lo trae
    try:
        with open("movimientos.txt", "r", encoding="utf-8-sig") as f:
            cadena = f.read()
    except OSError as e:
        print(f"movimientos.txt: {e.strerror}")
        return
    if not cadena.strip():
        print("movimientos.txt esta vacio.")
        return

    n = contar_movimientos(cadena)

    # Medir solo la verificacion, 'reps' veces
    tiempos = []
    msg = ""
    for _ in range(reps):
        t0 = time.perf_counter()
        _, msg = validar(inicio, cadena)
        tiempos.append(time.perf_counter() - t0)
    total = sum(tiempos)

    lineas = [
        "Lenguaje: Python",
        f"Posicion inicial: {inicio} | Movimientos: {n} | Resultado: {msg}",
    ]
    for i, t in enumerate(tiempos, 1):
        lineas.append(f"Repeticion {i}: {t * 1e6:.2f} microsegundos")
    lineas.append(f"Total {reps} repeticiones: {total * 1e6:.2f} microsegundos")
    lineas.append(f"Promedio: {total / reps * 1e6:.2f} microsegundos")

    # Escribir el resultado en el txt
    guardado = True
    try:
        with open("resultados_ejercicio1_python.txt", "w", encoding="utf-8") as out:
            out.write("\n".join(lineas) + "\n")
    except OSError as e:
        guardado = False
        print(f"resultados_ejercicio1_python.txt: {e.strerror}")

    # Mostrar en pantalla (sin la linea "Lenguaje")
    print("\n".join(lineas[1:]))
    if guardado:
        print("(Guardado en resultados_ejercicio1_python.txt)")


def main():
    while True:
        print("\n===== CABALLO DE AJEDREZ =====")
        print("1. Verificar movimientos")
        print("2. Medir el tiempo")
        print("0. Salir")
        try:
            op = input("Opcion: ").strip()
        except EOFError:
            break
        if op == "1":
            opcion_verificar()
        elif op == "2":
            opcion_medir()
        elif op == "0":
            break
        else:
            print("Opcion invalida.")


if __name__ == "__main__":
    main()
