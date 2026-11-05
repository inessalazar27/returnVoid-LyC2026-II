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
    

