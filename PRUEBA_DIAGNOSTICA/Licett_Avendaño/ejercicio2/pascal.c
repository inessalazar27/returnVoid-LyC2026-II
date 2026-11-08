/*Ejercicio 2 - Triangulo de Pascal*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Con n=100 los coeficientes llegan a e29 no caben en long long
   Uso unsigned __int128 que llega a 3.4e38 */
typedef unsigned __int128 numero;
#define TOPE (~(numero)0)
#define N_MAXIMO 130

/* int128 no se puede imprimir con printf hay que sacar los digitos a mano */
void imprimir(numero n)
{
    char digitos[50];
    int cuantos = 0;
 
    if (n == 0) {
        printf("0");
        return;
    }
    while (n > 0) {
        digitos[cuantos] = '0' + (int)(n % 10);
        cuantos++;
        n = n/10;
    }
    while (cuantos > 0) {
        cuantos--;
        putchar(digitos[cuantos]);
    }
}


