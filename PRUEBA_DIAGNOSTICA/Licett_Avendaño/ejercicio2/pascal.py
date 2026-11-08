# Ejercicio 2 - Triangulo de Pascal
import time

def armarTriengulo(n):
    triangulo=[] #guarda todas las filas de 0 a n 
    for i in range(n+1):
        fila=[]
        fila.append(1) #agrega el 1 al inicio de la fila
        for j in range(1, i):
            fila.append(triangulo[i-1][j-1] + triangulo[i-1][j])
        if i>0:
            fila.append(1)
        triangulo.append(fila)
    return triangulo

