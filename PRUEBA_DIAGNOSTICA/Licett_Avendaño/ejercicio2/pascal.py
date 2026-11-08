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

def polinomio(fila):
    n=len(fila)-1
    partes=[]
    for k in range(n, -1,-1):
        coeficiente=fila[k]
        if k==0:
            partes.append(str(coeficiente))
        else:
            texto = "" if coeficiente==1 else str(coeficiente)
            texto+= "x" if k==1 else "x^"+str(k)
            partes.append(texto)
    return " + ".join(partes)

def parentesis(valor):
    if valor < 0:
        return "(" + str(valor) + ")"
    return str(valor)

def mostrarPasos(fila, x):
    n=len(fila)-1
    paso1=[]
    paso2=[]
    paso3=[]
    suma=0
    for k in range(n,-1,-1):
        potencia=1
        for _ in range(k):
            potencia=potencia*x
        termino = fila[k]*potencia
        paso1.append(str(fila[k])+ "*" + parentesis(x)+"^"+str(k))
        paso2.append(str(fila[k]) + "*"+parentesis(potencia))
        paso3.append(parentesis(termino))
        suma=suma+termino
    print("f(x) =", " + ".join(paso1))
    print("     =", " + ".join(paso2))
    print("     =", " + ".join(paso3))
    print("     =", suma)
    return suma