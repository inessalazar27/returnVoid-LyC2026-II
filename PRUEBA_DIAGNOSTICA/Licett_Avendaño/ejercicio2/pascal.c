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

/*arreglo de punteros*/
numero **armarTriangulo(int n)
{
    numero **tri = malloc((n+1)*sizeof(numero *));
    for(int i=0; i<=n;i++){
        tri[i]=malloc((i+1)*sizeof(numero));
        tri[i][0]=1;
        tri[i][i]=1;
        for(int j=1; j<i; j++){
            tri[i][j]= tri[i-1][j-1]+ tri[i-1][j];
        }
    }
    return tri;
}

void liberarTriangulo(numero **tri, int n){
    for(int i=0; i<=n; i++){
        free(tri[i]);
    }
    free(tri);
}

void mostrarPolinomio(numero *fila, int n){
    for(int k=n; k>=0; k--){
        if(fila[k] !=1 || k==0){
            imprimir(fila[k]);
        }
        if (k>=1) printf("x");
        if (k>1) printf("^%d", k);
        if (k>0) printf(" + ");
    }
    printf("\n");
}

/*si algun paso pasa de 128 bits reduelve 0*/
int mostrarPasos(numero *fila, int n, unsigned int x){
    numero *potencia = malloc((n+1)*sizeof(numero));
    numero *termino = malloc((n+1)*sizeof(numero));
    numero suma=0;
    int cabe=1;

    for(int k=0; k<=n && cabe; k++){
        numero p=1;
        for (int veces =0; veces<k; veces++){
            if(x !=0 && p>TOPE/x){
                cabe=0;
                break;
            }
            p=p*x;
        }
        if(!cabe) break;
        if(p!=0 && fila[k]>TOPE/p){
            cabe=0;
            break;
        }
        potencia[k]=p;
        termino[k]=fila[k]*p;
        if(suma>TOPE-termino[k]){
            cabe=0;
            break;
        }
        suma=suma+termino[k];
    }
    if(!cabe){
        printf("el resultado se desborda no cabe en 128bits \n");
        free(potencia);
        free(termino);
        return 0;
    }
    printf("f(x) = ");
    for(int k =n; k>=0; k--){
        imprimir(fila[k]);
        printf("*%u^%d", x, k);
        if (k>0) printf(" + ");
    }
    printf("\n     = ");
    for(int k=n; k>=0; k--){
        imprimir(fila[k]);
        printf("*");
        imprimir(potencia[k]);
        if (k>0) printf(" + ");
    }
    printf("\n     = ");
    for(int k=n; k>=0; k--){
        imprimir(termino[k]);
        if (k>0) printf(" + ");
    }
    printf("\n     = ");
    imprimir(suma);
    printf("\n");
    free(potencia);
    free(termino);
    return 1;
}

int main(void){
    int n;
    unsigned int x;
    printf("valor de n (entero >=0, maximo %d): ",  N_MAXIMO);
    if (scanf("%d", &n) != 1) {
        printf("entrada invalida\n");
        return 1;
    }
    printf("valor de x (entero >=0): ");
    if (scanf("%u", &x) != 1) {
        printf("entrada invalida\n");
        return 1;
    }

    if (n < 0 || n > N_MAXIMO) {
        printf("n fuera de rango\n");
        return 1;
    }
    numero **triangulo = armarTriangulo(n);
    if(n<=10){
        printf("\nTriangulo de Pascal:\n");
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= i; j++) {
                imprimir(triangulo[i][j]);
                printf(" ");
            }
            printf("\n");
        }
    }
    printf("\nCoeficientes (n = %d): ", n);
    for (int j = 0; j <= n; j++) {
        imprimir(triangulo[n][j]);
        printf(" ");
    }
    printf("\n(x+1)^%d = ", n);
    mostrarPolinomio(triangulo[n], n);
 
    printf("\nCalculo paso a paso para x = %u\n", x);
    mostrarPasos(triangulo[n], n, x);
 
    liberarTriangulo(triangulo, n);
 
    /* tiempo de generar n=100 repetido 1000 veces para poder medirlo */
    int veces = 1000;
    clock_t inicio = clock();
    for (int r = 0; r < veces; r++) {
        numero **t = armarTriangulo(100);
        liberarTriangulo(t, 100);
    }
    clock_t fin = clock();
    double total_ms = (double)(fin - inicio) * 1000.0 / CLOCKS_PER_SEC;
 
    printf("\n--- Tiempo para generar n=100 (C) ---\n");
    printf("Repeticiones: %d\n", veces);
    printf("Tiempo total: %.3f ms\n", total_ms);
    printf("Tiempo por generacion: %.4f ms\n", total_ms / veces);
    return 0;
}


