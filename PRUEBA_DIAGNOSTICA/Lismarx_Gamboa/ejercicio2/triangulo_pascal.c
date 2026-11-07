/*
 * Dado un numero entero no negativo n:
 *   a) genera los coeficientes del polinomio (x+1)^n y muestra el resultado
 *   b) muestra por pasos el calculo de f(x) = (x+1)^n para un x dado
 * Se usa el triangulo de Pascal y memoria dinamica (malloc / free).
 *
 * Como para n=100 los numeros no caben en long long, se implementa un
 * entero grande (BigInt) con limbs de base 10^9 almacenados en memoria dinamica.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define BASE 1000000000u

/* Entero grande, c no tiene enteros tan grandes como los coeficientes entonces se crea una estructura para guardar numeros grandes */
typedef struct
{
    int sign;    /* 1 o -1 (el cero usa n = 0) */
    int n;       /* cantidad de limbs usados */
    uint32_t *d; /* limbs, el menos significativo primero */
} Big;

static void *reservar(size_t bytes)
{
    void *p = malloc(bytes ? bytes : 1);
    if (p == NULL)
    {
        fprintf(stderr, "Error: no hay memoria suficiente\n");
        exit(1);
    }
    return p;
}

static Big big_nuevo(int capacidad)
{
    Big b;
    b.sign = 1;
    b.n = 0;
    b.d = (uint32_t *)reservar(capacidad * sizeof(uint32_t));
    return b;
}

static void big_liberar(Big *b)
{
    free(b->d);
    b->d = NULL;
    b->n = 0;
}

static void big_recortar(Big *b)
{
    while (b->n > 0 && b->d[b->n - 1] == 0)
        b->n--;
    if (b->n == 0)
        b->sign = 1;
}

static Big big_desde_ll(long long v)
{
    Big b = big_nuevo(3);
    unsigned long long m;
    if (v < 0)
    {
        b.sign = -1;
        m = (unsigned long long)(-(v + 1)) + 1ULL;
    }
    else
        m = (unsigned long long)v;
    while (m > 0)
    {
        b.d[b.n++] = (uint32_t)(m % BASE);
        m /= BASE;
    }
    big_recortar(&b);
    return b;
}

static Big big_copiar(const Big *a)
{
    Big b = big_nuevo(a->n > 0 ? a->n : 1);
    memcpy(b.d, a->d, a->n * sizeof(uint32_t));
    b.n = a->n;
    b.sign = a->sign;
    return b;
}

static int cmp_mag(const Big *a, const Big *b)
{
    if (a->n != b->n)
        return a->n > b->n ? 1 : -1;
    for (int i = a->n - 1; i >= 0; i--)
        if (a->d[i] != b->d[i])
            return a->d[i] > b->d[i] ? 1 : -1;
    return 0;
}

static Big add_mag(const Big *a, const Big *b)
{
    int max = a->n > b->n ? a->n : b->n;
    Big r = big_nuevo(max + 1);
    uint32_t carry = 0;
    for (int i = 0; i < max; i++)
    {
        uint64_t s = (uint64_t)carry;
        if (i < a->n)
            s += a->d[i];
        if (i < b->n)
            s += b->d[i];
        r.d[i] = (uint32_t)(s % BASE);
        carry = (uint32_t)(s / BASE);
    }
    r.n = max;
    if (carry)
        r.d[r.n++] = carry;
    return r;
}

/* |a| >= |b| */
static Big sub_mag(const Big *a, const Big *b)
{
    Big r = big_nuevo(a->n);
    int64_t prestamo = 0;
    for (int i = 0; i < a->n; i++)
    {
        int64_t s = (int64_t)a->d[i] - prestamo - (i < b->n ? (int64_t)b->d[i] : 0);
        if (s < 0)
        {
            s += BASE;
            prestamo = 1;
        }
        else
            prestamo = 0;
        r.d[i] = (uint32_t)s;
    }
    r.n = a->n;
    big_recortar(&r);
    return r;
}

static Big big_sumar(const Big *a, const Big *b)
{
    Big r;
    if (a->sign == b->sign)
    {
        r = add_mag(a, b);
        r.sign = a->sign;
    }
    else
    {
        int c = cmp_mag(a, b);
        if (c == 0)
        {
            r = big_nuevo(1);
        }
        else if (c > 0)
        {
            r = sub_mag(a, b);
            r.sign = a->sign;
        }
        else
        {
            r = sub_mag(b, a);
            r.sign = b->sign;
        }
    }
    big_recortar(&r);
    return r;
}

static Big big_multiplicar(const Big *a, const Big *b)
{
    if (a->n == 0 || b->n == 0)
        return big_nuevo(1);
    Big r = big_nuevo(a->n + b->n);
    memset(r.d, 0, (a->n + b->n) * sizeof(uint32_t));
    r.n = a->n + b->n;
    for (int i = 0; i < a->n; i++)
    {
        uint64_t carry = 0;
        for (int j = 0; j < b->n || carry; j++)
        {
            uint64_t cur = r.d[i + j] + carry;
            if (j < b->n)
                cur += (uint64_t)a->d[i] * b->d[j];
            r.d[i + j] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
    }
    r.sign = a->sign * b->sign;
    big_recortar(&r);
    return r;
}

/* base^exp con exp >= 0 */
static Big big_potencia(const Big *base, int exp)
{
    Big r = big_desde_ll(1);
    for (int i = 0; i < exp; i++)
    {
        Big t = big_multiplicar(&r, base);
        big_liberar(&r);
        r = t;
    }
    return r;
}

static int big_mayor_que_1(const Big *a)
{
    return a->sign > 0 && (a->n > 1 || (a->n == 1 && a->d[0] > 1));
}

static void big_imprimir(const Big *a)
{
    if (a->n == 0)
    {
        printf("0");
        return;
    }
    if (a->sign < 0)
        printf("-");
    printf("%u", a->d[a->n - 1]);
    for (int i = a->n - 2; i >= 0; i--)
        printf("%09u", a->d[i]);
}

void triangulo_pascal(int num, long long x)
{
    if (num < 0)
    {
        printf("El numero no puede ser menor a 0\n");
        return;
    }

    int cantidad_terminos = 1;
    Big *linea_anterior = NULL; /* equivale a linea_anterior = [] */
    Big *linea_actual = NULL;
    int cant_anterior = 0;
    Big resultados = big_desde_ll(0);
    Big big_x = big_desde_ll(x);

    printf("exponente %d\n", num);

    for (int n = 0; n <= num; n++)
    {
        linea_actual = (Big *)reservar(cantidad_terminos * sizeof(Big));
        int exponente = cantidad_terminos - 1;
        int exp_iterar = cantidad_terminos - 1;

        if (linea_anterior == NULL)
        {
            linea_actual[0] = big_desde_ll(1);
        }
        else
        {
            int index = 0;
            for (int t = 0; t < cantidad_terminos; t++)
            {
                if (index == 0)
                    linea_actual[index] = big_copiar(&linea_anterior[0]);
                else if (index == cantidad_terminos - 1)
                    linea_actual[index] = big_copiar(&linea_anterior[cant_anterior - 1]);
                else
                    linea_actual[index] = big_sumar(&linea_anterior[index - 1], &linea_anterior[index]);
                index++;
            }
            /* liberar la linea anterior */
            for (int t = 0; t < cant_anterior; t++)
                big_liberar(&linea_anterior[t]);
            free(linea_anterior);
        }

        linea_anterior = linea_actual;
        cant_anterior = cantidad_terminos;

        /* Mostrar el polinomio de esta linea */
        for (int t = 0; t < cantidad_terminos; t++)
        {
            if (exp_iterar > 0)
            {
                if (big_mayor_que_1(&linea_actual[t]))
                    big_imprimir(&linea_actual[t]);
                printf("x^%d + ", exp_iterar);
                exp_iterar--;
            }
            else
            {
                big_imprimir(&linea_actual[t]);
                printf("\n");
            }
        }

        /* Evaluar solo la ultima linea */
        if (cantidad_terminos == num + 1 || num == 0)
        {
            printf("Evaluar [");
            for (int t = 0; t < cantidad_terminos; t++)
            {
                big_imprimir(&linea_actual[t]);
                if (t < cantidad_terminos - 1)
                    printf(", ");
            }
            printf("]\n");

            for (int t = 0; t < cantidad_terminos; t++)
            {
                Big termino;
                if (exponente > 0)
                {
                    Big pot = big_potencia(&big_x, exponente);
                    termino = big_multiplicar(&linea_actual[t], &pot);
                    big_liberar(&pot);

                    printf("operacion ");
                    big_imprimir(&linea_actual[t]);
                    printf("*%lld^%d = ", x, exponente);
                    big_imprimir(&termino);
                    printf("\n");
                    exponente--;
                }
                else
                {
                    termino = big_copiar(&linea_actual[t]);
                    printf("operacion ");
                    big_imprimir(&linea_actual[t]);
                    printf(" = ");
                    big_imprimir(&termino);
                    printf("\n");
                }
                Big nuevo = big_sumar(&resultados, &termino);
                big_liberar(&resultados);
                resultados = nuevo;
                big_liberar(&termino);
            }
            printf("Resultado Final: ");
            big_imprimir(&resultados);
            printf("\n");
        }

        cantidad_terminos++;
    }

    /* Liberar la ultima linea y el resto de memoria */
    for (int t = 0; t < cant_anterior; t++)
        big_liberar(&linea_anterior[t]);
    free(linea_anterior);
    big_liberar(&resultados);
    big_liberar(&big_x);
}

/* Lee un entero desde teclado; devuelve 1 si es valido, 0 si no */
int leer_entero(const char *mensaje, long long *valor)
{
    char buffer[64];
    char *fin;

    printf("%s", mensaje);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return 0;

    *valor = strtoll(buffer, &fin, 10);
    if (fin == buffer)
        return 0;
    while (*fin == ' ' || *fin == '\n' || *fin == '\r')
        fin++;
    if (*fin != '\0')
        return 0;

    printf("%lld\n", *valor);
    return 1;
}

int main(void)
{
    struct timespec inicio, fin;
    timespec_get(&inicio, TIME_UTC);

    int seguir = 1;
    while (seguir)
    {
        long long exponente, valor_x;

        if (!leer_entero("Ingrese el numero del exponente para evaluar el triangulo de pascal: ", &exponente))
        {
            printf("Error, numero no valido\n");
            continue;
        }

        if (!leer_entero("Ingrese el valor de x para evaluar el triangulo de pascal: ", &valor_x))
        {
            printf("Error, numero no valido\n");
            continue;
        }

        triangulo_pascal((int)exponente, valor_x);
        seguir = 0;
    }

    timespec_get(&fin, TIME_UTC);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Tiempo de ejecucion del programa: %fs\n", tiempo);

    return 0;
}