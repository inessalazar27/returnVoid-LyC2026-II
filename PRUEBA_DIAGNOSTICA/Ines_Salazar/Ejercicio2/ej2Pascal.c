#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Genera la fila n del triangulo de Pascal en memoria dinamica */
double *generar_fila(int n) {
    int i, j;
    double *fila = (double *) malloc((n + 1) * sizeof(double));

    fila[0] = 1;
    for (i = 1; i <= n; i++) {
        fila[i] = 1;                       /* el ultimo de cada fila siempre es 1 */
        for (j = i - 1; j >= 1; j--) {
            fila[j] = fila[j] + fila[j - 1];   /* suma de los dos de arriba */
        }
    }
    return fila;
}

int main() {
    int n, k, p, rep, r;
    double x, total, valor;
    double *coef;
    clock_t inicio, fin;
    double promedio;
    FILE *archivo;

    printf("Ingrese n: ");
    scanf("%d", &n);
    printf("Ingrese x: ");
    scanf("%lf", &x);

    archivo = fopen("resultado_c.txt", "w");

    /* a) Coeficientes y polinomio */
    coef = generar_fila(n);

    fprintf(archivo, "Coeficientes: ");
    for (k = 0; k <= n; k++) {
        fprintf(archivo, "%.0f ", coef[k]);
    }

    fprintf(archivo, "\n\nPolinomio: (x+1)^%d = ", n);
    for (k = 0; k <= n; k++) {
        p = n - k;                         /* el exponente baja de n a 0 */
        fprintf(archivo, "%.0f", coef[k]);
        if (p == 1) fprintf(archivo, "x");
        if (p > 1)  fprintf(archivo, "x^%d", p);
        if (k < n)  fprintf(archivo, " + ");
    }

    /* b) Calculo por pasos de f(x) */
    fprintf(archivo, "\n\nCalculo por pasos con x = %.2f:\n", x);
    total = 0;
    for (k = 0; k <= n; k++) {
        p = n - k;
        valor = coef[k] * pow(x, p);
        total = total + valor;
        fprintf(archivo, "Paso %d: %.0f * (%.2f)^%d = %.0f\n", k + 1, coef[k], x, p, valor);
    }
    fprintf(archivo, "\nf(%.2f) = %.0f\n", x, total);
    fprintf(archivo, "Verificacion (x+1)^n = %.0f\n", pow(x + 1, n));

    free(coef);                            /* liberar la memoria dinamica */

    /* Medicion de tiempo para n = 100 (se repite 1000 veces) */
    rep = 1000;
    inicio = clock();
    for (r = 0; r < rep; r++) {
        coef = generar_fila(100);
        free(coef);
    }
    fin = clock();
    promedio = ((double)(fin - inicio) / CLOCKS_PER_SEC) / rep;

    fprintf(archivo, "\nTiempo C n=100: %.9f segundos (promedio de %d corridas)\n", promedio, rep);

    fclose(archivo);
    printf("Listo, revisa el archivo resultado_c.txt\n");
    return 0;
}