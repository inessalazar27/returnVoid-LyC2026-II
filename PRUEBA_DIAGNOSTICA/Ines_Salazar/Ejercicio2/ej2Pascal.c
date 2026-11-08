#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef unsigned __int128 u128;
#define N_MAX 128

static char buf[16384];

/*Genera la fila n del triangulo de Pascal en memoria dinamica */
u128 *generar_fila(int n) {
    int i, j;
    u128 *fila = (u128 *) malloc((n + 1) * sizeof(u128));
    if (fila == NULL) return NULL;

    fila[0] = 1;
    for (i = 1; i <= n; i++) {
        fila[i] = 1;                            /* el ultimo de cada fila es 1 */
        for (j = i - 1; j >= 1; j--)            /* de derecha a izquierda para no pisar valores */
            fila[j] = fila[j] + fila[j - 1];
    }
    return fila;
}

/*Convierte un u128 a texto.printf no lo soporta. Devuelve la longitud */
int u128_a_texto(u128 v, char *out) {
    char tmp[64];
    int len = 0, i;
    if (v == 0) { out[0] = '0'; out[1] = '\0'; return 1; }
    while (v > 0) { tmp[len++] = '0' + (int)(v % 10); v /= 10; }
    for (i = 0; i < len; i++) out[i] = tmp[len - 1 - i];
    out[len] = '\0';
    return len;
}

/* Arma (x+1)^n =.. */
void polinomio_a_texto(char *out, const u128 *f, int n, const char *sep) {
    int k, p, len;
    char num[64];
    len = sprintf(out, "(x+1)^%d = ", n);
    for (k = 0; k <= n; k++) {
        p = n - k;
        if (f[k] != 1 || p == 0) {              /* no se escribe el coeficiente 1 */
            u128_a_texto(f[k], num);
            len += sprintf(out + len, "%s", num);
        }
        if (p == 1)      len += sprintf(out + len, "x");
        else if (p > 1)  len += sprintf(out + len, "x^%d", p);
        if (k < n)       len += sprintf(out + len, "%s", sep);
    }
}

/* Imprime polinomio izquierda y fila de Pascal derecha para i = 0..n */
void imprimir_triangulo(FILE *archivo, int n) {
    int i, j;
    char num[64];
    u128 *f;

    fprintf(archivo, "Triangulo de Pascal y polinomios:\n\n");
    for (i = 0; i <= n; i++) {
        f = generar_fila(i);
        if (f == NULL) return;
        polinomio_a_texto(buf, f, i, "+");
        fprintf(archivo, "%-76s(n =%d): ", buf, i);
        for (j = 0; j < (n - i) * 2; j++) fputc(' ', archivo);
        for (j = 0; j <= i; j++) {
            u128_a_texto(f[j], num);
            fprintf(archivo, "%4s", num);
        }
        fputc('\n', archivo);
        free(f);
    }
    fputc('\n', archivo);
}

int main() {
    int n, k, p, rep, r;
    double x, total, valor;
    u128 *coef;
    char num[64];
    clock_t inicio, fin;
    double promedio;
    volatile u128 sumidero = 0;     /* evita que -O2 elimine el malloc/free medido */
    FILE *archivo;

    printf("Ingrese n (0 a %d): ", N_MAX);
    if (scanf("%d", &n) != 1 || n < 0 || n > N_MAX) {
        printf("n invalido: debe ser un entero entre 0 y %d\n", N_MAX);
        return 1;
    }
    printf("Ingrese x: ");
    if (scanf("%lf", &x) != 1) {
        printf("x invalido\n");
        return 1;
    }

    archivo = fopen("resultado_c.txt", "w");
    if (archivo == NULL) {
        printf("No se pudo crear resultado_c.txt\n");
        return 1;
    }

    imprimir_triangulo(archivo, n <= 10 ? n : 10);

    /*Coeficientes y polinomio */
    coef = generar_fila(n);
    if (coef == NULL) {
        printf("Sin memoria\n");
        fclose(archivo);
        return 1;
    }

    fprintf(archivo, "Coeficientes: ");
    for (k = 0; k <= n; k++) {
        u128_a_texto(coef[k], num);
        fprintf(archivo, "%s ", num);
    }
    polinomio_a_texto(buf, coef, n, " + ");
    fprintf(archivo, "\n\nPolinomio: %s\n", buf);

    /*Calculo por pasos de f(x). Los terminos se calculan en long double:
          son exactos solo mientras quepan en 64 bits de mantisa */
    fprintf(archivo, "\nCalculo por pasos con x = %g:\n", x);
    total = 0;
    for (k = 0; k <= n; k++) {
        p = n - k;
        valor = (double) coef[k] * pow(x, p);
        total += valor;
        u128_a_texto(coef[k], num);
        fprintf(archivo, "Paso %d: %s * (%g)^%d = %.10g\n", k + 1, num, x, p, valor);
    }
    fprintf(archivo, "\nf(%g) = %.10g\n", x, total);
    fprintf(archivo, "Verificacion (x+1)^n = %.10g\n", pow(x + 1, n));

    free(coef);

    /*Medicion de tiempo para n = 100 */
    rep = 1000;
    inicio = clock();
    for (r = 0; r < rep; r++) {
        coef = generar_fila(100);
        sumidero += coef[50];
        free(coef);
    }
    fin = clock();
    promedio = ((double)(fin - inicio) / CLOCKS_PER_SEC) / rep;

    fprintf(archivo, "\nTiempo C n=100: %.9f segundos (promedio de %d corridas)\n", promedio, rep);

    fclose(archivo);
    printf("Listo, revisa el archivo resultado_c.txt\n");
    return 0;
}
