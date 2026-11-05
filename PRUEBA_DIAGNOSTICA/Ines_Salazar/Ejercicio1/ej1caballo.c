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
    char letra = mov[0];
    if (letra == 'k') letra = 'K';              /* acepta k minuscula */

    if (letra != 'K') {
        snprintf(msg, sz, "movimiento INCORRECTO falla letra %c no pertenece a un caballo.", mov[0]);
        return 0;
    }

    if (strlen(mov) != 3) {
        snprintf(msg, sz, "INCORRECTO falla en el movimiento %d.", num);
        return 0;
    }

    char cc = mov[1];
    if (cc >= 'A' && cc <= 'H') cc += 32;       /* acepta columna en mayuscula */

    if (cc < 'a' || cc > 'h' || mov[2] < '1' || mov[2] > '8') {
        snprintf(msg, sz, "INCORRECTO falla en el movimiento %d.", num);
        return 0;
    }

    int c = cc - 'a' + 1;
    int f = mov[2] - '0';

    if (abs(c - *col) * abs(f - *fila) != 2) {
        snprintf(msg, sz, "INCORRECTO falla en el movimiento %d.", num);
        return 0;
    }

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
/* Opcion 1 , el usuario escribe posicion, n y movimientos; se verifica */
static void opcion_verificar(void) {
    char inicio[3], msg[128], entrada[65536], linea[32];
    int n;
    printf("\n--- VERIFICAR MOVIMIENTOS ---\n");
    leer_posicion(inicio);

    for (;;) {
        printf("Cantidad de movimientos n: ");
        if (fgets(linea, sizeof linea, stdin) == NULL) return;
        n = atoi(linea);
        if (n > 0) break;
        printf("n debe ser un numero mayor que 0.\n");
    }

    for (;;) {
    printf("Escriba los %d movimientos (ej. Kc2 Kd4 Kb3): ", n);
    if (fgets(entrada, sizeof entrada, stdin) == NULL) return;

    /* si no hay salto de linea, la entrada no cupo: avisar y limpiar */
    if (strchr(entrada, '\n') == NULL && !feof(stdin)) {
        printf("Entrada demasiado larga. Use la opcion 2 con movimientos.txt.\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { }
        continue;
    }

    entrada[strcspn(entrada, "\r\n")] = 0;
    int real = contar_movimientos(entrada);
    if (real == n) break;
    printf("Indico n=%d pero escribio %d movimientos. Intente de nuevo.\n", n, real);
}

    validar(inicio, entrada, msg, sizeof msg);
    printf("\nPara Ak = %s n= %d y C= %s\n", inicio, n, entrada);
    printf("salida: %s\n", msg);
}
/* Opcion 2: medir el tiempo con movimientos.txt */
static void opcion_medir(void) {
    char inicio[3], msg[128], linea[32];
    int reps = 10;
    printf("\n--- MEDIR TIEMPO (movimientos.txt) ---\n");
    leer_posicion(inicio);

    printf("Cuantas veces verificar? (Enter = 10): ");
    if (fgets(linea, sizeof linea, stdin) != NULL && atoi(linea) > 0)
        reps = atoi(linea);

    /* Cargar el archivo en memoria dinamica */
    FILE *f = fopen("movimientos.txt", "rb");
    if (!f) { perror("movimientos.txt"); return; }
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);
    if (tam <= 0) { printf("movimientos.txt esta vacio.\n"); fclose(f); return; }
    char *cadena = malloc((size_t)tam + 1);
    if (cadena == NULL) { printf("Sin memoria.\n"); fclose(f); return; }
    if (fread(cadena, 1, (size_t)tam, f) != (size_t)tam) {
        printf("Error leyendo movimientos.txt.\n");
        fclose(f); free(cadena); return;
    }
    cadena[tam] = 0;
    fclose(f);

    int n = contar_movimientos(cadena);

    /* Medir solo la verificacion, 'reps' veces */
    double *tiempos = malloc((size_t)reps * sizeof(double));
    if (tiempos == NULL) { printf("Sin memoria.\n"); free(cadena); return; }
    double total = 0;
    for (int r = 0; r < reps; r++) {
        double t0 = ahora();
        validar(inicio, cadena, msg, sizeof msg);
        tiempos[r] = ahora() - t0;
        total += tiempos[r];
    }

    /* Escribir el resultado en el txt */
    FILE *out = fopen("resultados_ejercicio1_c.txt", "w");
    if (out) {
        fprintf(out, "Lenguaje: C\n");
        fprintf(out, "Posicion inicial: %s | Movimientos: %d | Resultado: %s\n", inicio, n, msg);
        for (int r = 0; r < reps; r++)
            fprintf(out, "Repeticion %d: %.2f microsegundos\n", r + 1, tiempos[r] * 1e6);
        fprintf(out, "Total %d repeticiones: %.2f microsegundos\n", reps, total * 1e6);
        fprintf(out, "Promedio: %.2f microsegundos\n", total / reps * 1e6);
        fclose(out);
    } else {
        perror("resultados_ejercicio1_c.txt");
    }

    /* Mostrar en pantalla */
    printf("Posicion inicial: %s | Movimientos: %d | Resultado: %s\n", inicio, n, msg);
    for (int r = 0; r < reps; r++)
        printf("Repeticion %d: %.2f microsegundos\n", r + 1, tiempos[r] * 1e6);
    printf("Total %d repeticiones: %.2f microsegundos\n", reps, total * 1e6);
    printf("Promedio: %.2f microsegundos\n", total / reps * 1e6);
    if (out) printf("(Guardado en resultados_ejercicio1_c.txt)\n");

    free(tiempos);
    free(cadena);
}

int main(void) {
char linea[16];
    for (;;) {
        printf("\n===== CABALLO DE AJEDREZ =====\n");
        printf("1. Verificar movimientos\n");
        printf("2. Medir el tiempo\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        if (fgets(linea, sizeof linea, stdin) == NULL) break;
        switch (linea[0]) {
            case '1': opcion_verificar(); break;
            case '2': opcion_medir(); break;
            case '0': return 0;
            default: printf("Opcion invalida.\n");
        }
    }
    return 0;
}