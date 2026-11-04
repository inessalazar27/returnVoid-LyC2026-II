/* Ejercicio 1: validador de movimientos de caballo.
   Metodo: columnas movidas x filas movidas = 2  ->  movimiento de caballo                                */
#ifndef _WIN32
#define _POSIX_C_SOURCE 199309L   /* para clock_gettime en Linux/Mac */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif
/* Cronometro de alta precision (segundos), portable */
/* Cronometro de alta precision (segundos), portable */
static double ahora(void) {
#ifdef _WIN32
    static LARGE_INTEGER frec = {0};          /* la frecuencia se pide una sola vez */
    LARGE_INTEGER cont;
    if (frec.QuadPart == 0) QueryPerformanceFrequency(&frec);
    QueryPerformanceCounter(&cont);
    return (double)cont.QuadPart / (double)frec.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#endif
}
/* Revisa UN movimiento
   col y fila son la posicion actual del caballo; si el movimiento es
   correcto se actualizan a la nueva casilla. Devuelve 1 si es correcto. */
static int revisar(const char *mov, int num, int *col, int *fila, char *msg, size_t sz) {

    if (mov[0] != 'K') {
        snprintf(msg, sz, "movimiento INCORRECTO falla letra %c no pertenece a un caballo.", mov[0]);
        return 0;
    }
    
    if (strlen(mov) != 3 || mov[1] < 'a' || mov[1] > 'h' || mov[2] < '1' || mov[2] > '8') {
        snprintf(msg, sz, "INCORRECTO falla en el movimiento %d.", num);
        return 0;
    }
    
    int c = mov[1] - 'a' + 1;   
    int f = mov[2] - '0';     

   
    int dc = abs(c - *col);
    int df = abs(f - *fila);

  
    if (dc * df != 2) {
        snprintf(msg, sz, "INCORRECTO falla en el movimiento %d.", num);
        return 0;
    }
    /* Correcto: el caballo queda en la casilla nueva */
    *col = c;
    *fila = f;
    return 1;
}
int main(void) {

    return 0;
}