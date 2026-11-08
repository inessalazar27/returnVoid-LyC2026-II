#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>

static FILE *archivo_resultados;

static void imprimir(const char *formato, ...) {
    va_list argumentos;
    va_list copia_argumentos;
    va_start(argumentos, formato);
    va_copy(copia_argumentos, argumentos);
    vprintf(formato, argumentos);
    vfprintf(archivo_resultados, formato, copia_argumentos);
    va_end(copia_argumentos);
    va_end(argumentos);
}

#define printf imprimir

// Genera los coeficientes de (x+1)^n mediante el Triangulo de Pascal usando memoria dinamica
double* generar_pascal_dinamico(int n, int mostrar_triangulo) {
    // Reserva dinamica para la fila 0: (x+1)^0 = 1
    double *fila_actual = (double*)malloc(1 * sizeof(double));
    if (!fila_actual) return NULL;
    fila_actual[0] = 1.0;

    if (mostrar_triangulo) {
        printf("\n--- Triangulo de Pascal hasta n = %d (Memoria Dinamica) ---\n", n);
        printf("(n = 0): 1\n");
    }

    for (int i = 1; i <= n; i++) {
        // Reserva dinamica para la fila i (i + 1 elementos)
        double *nueva_fila = (double*)malloc((i + 1) * sizeof(double));
        if (!nueva_fila) {
            free(fila_actual);
            return NULL;
        }

        nueva_fila[0] = 1.0;
        nueva_fila[i] = 1.0;

        for (int j = 1; j < i; j++) {
            nueva_fila[j] = fila_actual[j - 1] + fila_actual[j];
        }

        if (mostrar_triangulo) {
            printf("(n = %d): ", i);
            for (int j = 0; j <= i; j++) {
                printf("%.0f ", nueva_fila[j]);
            }
            printf("\n");
        }

        // Liberar la memoria de la fila anterior y apuntar a la nueva
        free(fila_actual);
        fila_actual = nueva_fila;
    }

    return fila_actual;
}

// Parte a): Muestra el polinomio generado (x+1)^n
void mostrar_polinomio(int n, double *coefs) {
    printf("\n--- a) Resultado del Polinomio (x+1)^%d ---\n", n);
    printf("(x+1)^%d = ", n);

    if (n == 0) {
        printf("1\n");
        return;
    }

    for (int i = 0; i <= n; i++) {
        int exp = n - i;
        double c = coefs[i];

        if (exp == 0) {
            printf("%.0f", c);
        } else if (exp == 1) {
            if (c == 1.0) printf("x + ");
            else printf("%.0fx + ", c);
        } else {
            if (c == 1.0) printf("x^%d + ", exp);
            else printf("%.0fx^%d + ", c, exp);
        }
    }
    printf("\n");
}

// Parte b): Muestra por pasos el calculo para x dado: f(x) = (x+1)^n
void calcular_paso_a_paso(int n, double x, double *coefs) {
    printf("\n--- b) Calculo paso a paso para x = %.2f en f(x) = (x+1)^%d ---\n", x, n);

    // Reservamos memoria dinamica para guardar los valores intermedios de cada paso
    double *potencias = (double*)malloc((n + 1) * sizeof(double));
    double *terminos  = (double*)malloc((n + 1) * sizeof(double));

    // Paso 1: Sustitucion de x en el polinomio
    printf("Paso 1 (Sustitucion):  f(%.2f) = ", x);
    for (int i = 0; i <= n; i++) {
        int exp = n - i;
        if (exp == 0) printf("%.0f", coefs[i]);
        else if (exp == 1) printf("%.0f*(%.2f) + ", coefs[i], x);
        else printf("%.0f*(%.2f^%d) + ", coefs[i], x, exp);
    }
    printf("\n");

    // Paso 2: Resolucion de potencias (x^k)
    printf("Paso 2 (Potencias):    f(%.2f) = ", x);
    for (int i = 0; i <= n; i++) {
        int exp = n - i;
        potencias[i] = pow(x, exp);
        if (exp == 0) printf("%.0f", coefs[i]);
        else printf("%.0f*(%.2f) + ", coefs[i], potencias[i]);
    }
    printf("\n");

    // Paso 3: Multiplicacion de coeficientes por potencias
    printf("Paso 3 (Productos):    f(%.2f) = ", x);
    double suma_total = 0.0;
    for (int i = 0; i <= n; i++) {
        terminos[i] = coefs[i] * potencias[i];
        suma_total += terminos[i];
        printf("%.2f%s", terminos[i], (i == n) ? "" : " + ");
    }
    printf("\n");

    // Paso 4: Suma total
    printf("Paso 4 (Resultado):    f(%.2f) = %.2f\n", x, suma_total);

    free(potencias);
    free(terminos);
}

int main() {
    int n;
    double x;

    archivo_resultados = fopen("resultado_ejercicio2_c.txt", "a");
    if (archivo_resultados == NULL) {
        perror("No se pudo abrir resultado_ejercicio2_c.txt");
        return 1;
    }
    fprintf(archivo_resultados, "\n=== Nueva ejecucion ===\n");

    printf("====================================================\n");
    printf("   EJERCICIO 2 - TRIANGULO DE PASCAL EN LENGUAJE C  \n");
    printf("====================================================\n");

    printf("Ingrese un numero entero no negativo n (ej. 4 o 5): ");
    if (scanf("%d", &n) != 1 || n < 0) {
        n = 5; // Valor por defecto en caso de entrada no interactiva
        printf("%d (valor por defecto)\n", n);
    }

    printf("Ingrese el valor de x para evaluar f(x) = (x+1)^%d (ej. 2): ", n);
    if (scanf("%lf", &x) != 1) {
        x = 2.0;
        printf("%.2f (valor por defecto)\n", x);
    }

    // 1. Generar coeficientes con memoria dinamica y mostrar pasos para n y x dados
    double *coefs = generar_pascal_dinamico(n, 1);
    if (coefs != NULL) {
        mostrar_polinomio(n, coefs);
        calcular_paso_a_paso(n, x, coefs);
        free(coefs); // Liberar memoria dinamica
    }

    // 2. Medir el tiempo necesario para generar n = 100 en C
    int n_benchmark = 100;
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    double *coefs_100 = generar_pascal_dinamico(n_benchmark, 0);

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_segundos = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("\n====================================================\n");
    printf("   MEDICION DE TIEMPO PARA n = 100 (LENGUAJE C)     \n");
    printf("====================================================\n");
    printf("Polinomio generado (x+1)^100: %.0fx^100 + %.0fx^99 + %.0fx^98 + ... + %.0fx + %.0f\n",
           coefs_100[0], coefs_100[1], coefs_100[2], coefs_100[99], coefs_100[100]);
    printf("Tiempo en generar n = 100 en C: %.8f segundos (%.3f microsegundos)\n",
           tiempo_segundos, tiempo_segundos * 1e6);

    free(coefs_100);
    fclose(archivo_resultados);
    return 0;
}