#Ejercicio 1 / Validador de moviemintos de caballo
import time
import os

COLUMNAS = "abcdefgh"
FILAS = "12345678"

"""Convierte la casilla G5 en (6,4) devuelve None si la casilla no existe"""
def coordenadas(casilla): 
    if len(casilla) != 2:
        return None
    columna, fila = casilla[0], casilla[1]
    if columna not in COLUMNAS or fila not in FILAS:
        return None
    return COLUMNAS.index(columna), int(fila)-1

"""el moviemto valido para el caballo"""
def esMovimientoValido(inicio, final):
    x= abs(final[0] - inicio[0])
    y = abs(final[1] - inicio[1])
    return (x==1 and y==2) or (x==2 and y==1)
    

def validar(inicio, cadena):
    pos= coordenadas(inicio)
    if pos is None:
        return f"ERROR: la posicion inicial {inicio} no existe"
    movimientos=cadena.split()
    for i, mov in enumerate(movimientos, start=1):
        if mov[0] != "k":
            return f"movimiento incorrecto la letra {mov[0]} no es un caballo"
        final = coordenadas(mov[1:])
        if final is None or not esMovimientoValido(pos, final):
            return f"ERROR falla en el moviemiento {i}"
        pos=final
    return "el movimiento es correcto"

def medir (inicio, cadena, rep):
    t0= time.perf_counter()
    for _ in range(rep):
        resultado = validar(inicio, cadena)
    t1 = time.perf_counter()
    return resultado, (t1-t0) * 1000 

if __name__ == "__main__":
    ejemplo = [
      ("a1", "Kc2 Kd4 Kb3 Kc5"),
      ("a1", "Kc2 Kd4 Kb4 Kc5"),
      ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7"),
      ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7"),
      ("a8", "Kb6 Kd7"),
      ("a8", "Kb6 Kb7"),
      ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
      ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
      ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"),  
    ]
    lineas = ["===Ejercicio 1===", "", ]
