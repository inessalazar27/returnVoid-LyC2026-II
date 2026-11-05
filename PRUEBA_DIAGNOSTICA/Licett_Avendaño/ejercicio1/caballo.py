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
    

