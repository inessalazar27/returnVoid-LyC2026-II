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
/* Copia una cadena en memoria dinamica (strtok modifica el texto).
   Si no hay memoria, termina el programa con un mensaje. */
static char *duplicar(const char *s) {
    char *copia = malloc(strlen(s) + 1);
    if (copia == NULL) {
        fprintf(stderr, "Error: sin memoria.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(copia, s);
    return copia;
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

/* Revisa una cadena de movimientos. Se detiene en el primer error. */
static int validar(const char *inicio, const char *cadena, char *msg, size_t sz) {
    int col = inicio[0] - 'a' + 1;
    int fila = inicio[1] - '0';
    char *copia = duplicar(cadena);

    int num = 0, ok = 1;
    for (char *tok = strtok(copia, " \t\r\n"); tok; tok = strtok(NULL, " \t\r\n")) {
        num++;
        if (!revisar(tok, num, &col, &fila, msg, sz)) { ok = 0; break; }
    }
    if (ok) snprintf(msg, sz, "movimiento CORRECTO.");
    free(copia);
    return ok;
}

/* Cuenta cuantos movimientos hay en una cadena */
static int contar_movimientos(const char *cadena) {
    char *copia = duplicar(cadena);
    int n = 0;
    for (char *t = strtok(copia, " \t\r\n"); t; t = strtok(NULL, " \t\r\n")) n++;
    free(copia);
    return n;
}
/* Pide la posicion inicial hasta que sea valida */
static void leer_posicion(char *dest) {
    char buf[64];
    for (;;) {
        printf("Posicion inicial (ej. a2): ");
        if (fgets(buf, sizeof buf, stdin) == NULL) { strcpy(dest, "a1"); return; }
        if (buf[0] >= 'A' && buf[0] <= 'H') buf[0] += 32;   /* mayuscula -> minuscula */
        if (buf[0] >= 'a' && buf[0] <= 'h' && buf[1] >= '1' && buf[1] <= '8' &&
            (buf[2] == '\n' || buf[2] == '\r' || buf[2] == '\0')) {
            dest[0] = buf[0]; dest[1] = buf[1]; dest[2] = 0;
            return;
        }
        printf("Posicion invalida. Use columna a-h y fila 1-8.\n");
    }
}
int main(void) {

    return 0;
}