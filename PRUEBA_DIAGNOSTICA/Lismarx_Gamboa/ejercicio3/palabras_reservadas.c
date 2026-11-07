/*
 * Ejercicio 3: Dado un programa escrito en C, cargarlo en memoria dinamica,
 * verificar si existen palabras reservadas de C y traducirlas al espanol.
 *
 * Salida: reporte por pantalla y el archivo "traducido.txt" con el codigo
 *         donde cada palabra reservada fue reemplazada por su equivalente.
 *         (No es compilable: es solo para ver la traduccion.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    const char *c;  /* palabra reservada en C */
    const char *es; /* equivalente en espanol */
} Palabra;

static const Palabra TABLA[] = {
    {"auto", "automatico"}, {"break", "romper"}, {"case", "caso"}, {"char", "caracter"}, {"const", "constante"}, {"continue", "continuar"}, {"default", "por_defecto"}, {"do", "hacer"}, {"double", "doble"}, {"else", "sino"}, {"enum", "enumeracion"}, {"extern", "externo"}, {"float", "flotante"}, {"for", "para"}, {"goto", "ir_a"}, {"if", "si"}, {"int", "entero"}, {"long", "largo"}, {"register", "registro"}, {"return", "retornar"}, {"short", "corto"}, {"signed", "con_signo"}, {"sizeof", "tamano_de"}, {"static", "estatico"}, {"struct", "estructura"}, {"switch", "segun"}, {"typedef", "definir_tipo"}, {"union", "union"}, {"unsigned", "sin_signo"}, {"void", "vacio"}, {"volatile", "volatil"}, {"while", "mientras"}, {"inline", "en_linea"}, {"restrict", "restringido"}, {"_Bool", "_Booleano"}, {"_Complex", "_Complejo"}, {"_Imaginary", "_Imaginario"}, {"_Alignas", "_Alinear_como"}, {"_Alignof", "_Alineacion_de"}, {"_Atomic", "_Atomico"}, {"_Generic", "_Generico"}, {"_Noreturn", "_Sin_retorno"}, {"_Static_assert", "_Asercion_estatica"}, {"_Thread_local", "_Local_al_hilo"}};

#define NUM_PAL ((int)(sizeof(TABLA) / sizeof(TABLA[0])))

/* Registro dinamico de apariciones de cada palabra */
typedef struct
{
    int cantidad;
    int capacidad;
    int *lineas; /* arreglo dinamico con las lineas donde aparece */
} Apariciones;

static void *reservar(size_t bytes)
{
    void *p = malloc(bytes);
    if (p == NULL)
    {
        fprintf(stderr, "Error: no hay memoria suficiente\n");
        exit(1);
    }
    return p;
}

/* Carga todo el archivo en un bloque de memoria dinamica */
static char *cargar_archivo(const char *ruta, long *tamano)
{
    FILE *f = fopen(ruta, "rb");
    if (f == NULL)
        return NULL;

    fseek(f, 0, SEEK_END);
    *tamano = ftell(f);
    rewind(f);

    char *buffer = (char *)reservar(*tamano + 1);
    size_t leidos = fread(buffer, 1, *tamano, f);
    buffer[leidos] = '\0';
    *tamano = (long)leidos;
    fclose(f);
    return buffer;
}

/* Devuelve el indice en tabla o -1 si no es palabra reservada */
static int buscar_palabra(const char *palabra)
{
    for (int i = 0; i < NUM_PAL; i++)
        if (strcmp(palabra, TABLA[i].c) == 0)
            return i;
    return -1;
}

static void registrar(Apariciones *a, int linea)
{
    if (a->cantidad == a->capacidad)
    {
        a->capacidad = a->capacidad ? a->capacidad * 2 : 4;
        a->lineas = (int *)realloc(a->lineas, a->capacidad * sizeof(int));
        if (a->lineas == NULL)
        {
            fprintf(stderr, "Error: no hay memoria suficiente\n");
            exit(1);
        }
    }
    a->lineas[a->cantidad++] = linea;
}

/* Recorre el texto cargado en memoria. Ignora comentarios, cadenas y caracteres. */
static void analizar(const char *texto, long tam, Apariciones *ap, FILE *salida)
{
    enum
    {
        NORMAL,
        COMENT_LINEA,
        COMENT_BLOQUE,
        CADENA,
        CARACTER
    } estado = NORMAL;
    int linea = 1;
    long i = 0;

    char *palabra = (char *)reservar(tam + 1); /* buffer  para cada palabra */

    while (i < tam)
    {
        char c = texto[i];

        switch (estado)
        {
        case NORMAL:
            if (c == '/' && i + 1 < tam && texto[i + 1] == '/')
            {
                estado = COMENT_LINEA;
                fputs("//", salida);
                i += 2;
            }
            else if (c == '/' && i + 1 < tam && texto[i + 1] == '*')
            {
                estado = COMENT_BLOQUE;
                fputs("/*", salida);
                i += 2;
            }
            else if (c == '"')
            {
                estado = CADENA;
                fputc(c, salida);
                i++;
            }
            else if (c == '\'')
            {
                estado = CARACTER;
                fputc(c, salida);
                i++;
            }
            else if (isalpha((unsigned char)c) || c == '_')
            {
                long j = 0;
                while (i < tam && (isalnum((unsigned char)texto[i]) || texto[i] == '_'))
                    palabra[j++] = texto[i++];
                palabra[j] = '\0';

                int k = buscar_palabra(palabra);
                if (k >= 0)
                {
                    registrar(&ap[k], linea);
                    fputs(TABLA[k].es, salida);
                }
                else
                    fputs(palabra, salida);
            }
            else if (isdigit((unsigned char)c))
            {
                /* numeros (ej. 1e5, 0x1F, 10L) se copian completos */
                while (i < tam && (isalnum((unsigned char)texto[i]) || texto[i] == '.'))
                    fputc(texto[i++], salida);
            }
            else
            {
                if (c == '\n')
                    linea++;
                fputc(c, salida);
                i++;
            }
            break;

        case COMENT_LINEA:
            if (c == '\n')
            {
                estado = NORMAL;
                linea++;
            }
            fputc(c, salida);
            i++;
            break;

        case COMENT_BLOQUE:
            if (c == '*' && i + 1 < tam && texto[i + 1] == '/')
            {
                estado = NORMAL;
                fputs("*/", salida);
                i += 2;
            }
            else
            {
                if (c == '\n')
                    linea++;
                fputc(c, salida);
                i++;
            }
            break;

        case CADENA:
        case CARACTER:
            if (c == '\\' && i + 1 < tam)
            { /* secuencia de escape: copiar los dos caracteres */
                fputc(c, salida);
                fputc(texto[i + 1], salida);
                i += 2;
            }
            else
            {
                if ((estado == CADENA && c == '"') || (estado == CARACTER && c == '\''))
                    estado = NORMAL;
                if (c == '\n')
                    linea++;
                fputc(c, salida);
                i++;
            }
            break;
        }
    }
    free(palabra);
}

int main(int argc, char *argv[])
{
    char ruta[256];

    if (argc > 1)
    {
        strncpy(ruta, argv[1], sizeof(ruta) - 1);
        ruta[sizeof(ruta) - 1] = '\0';
    }
    else
    {
        printf("Ingrese la ruta del archivo .c a analizar: ");
        if (fgets(ruta, sizeof(ruta), stdin) == NULL)
            return 1;
        ruta[strcspn(ruta, "\r\n")] = '\0';
    }

    long tam = 0;
    char *texto = cargar_archivo(ruta, &tam);
    if (texto == NULL)
    {
        printf("Error: no se pudo abrir el archivo '%s'\n", ruta);
        return 1;
    }
    printf("Archivo '%s' cargado en memoria dinamica (%ld bytes)\n\n", ruta, tam);

    Apariciones *ap = (Apariciones *)calloc(NUM_PAL, sizeof(Apariciones));
    if (ap == NULL)
    {
        free(texto);
        return 1;
    }

    FILE *salida = fopen("traducido.txt", "w");
    if (salida == NULL)
    {
        printf("Error: no se pudo crear traducido.txt\n");
        free(ap);
        free(texto);
        return 1;
    }

    analizar(texto, tam, ap, salida);
    fclose(salida);

    /* Reporte */
    int distintas = 0, total = 0;
    printf("%-15s %-20s %-8s %s\n", "PALABRA C", "ESPANOL", "VECES", "LINEAS");
    printf("-------------------------------------------------------------------\n");
    for (int k = 0; k < NUM_PAL; k++)
    {
        if (ap[k].cantidad == 0)
            continue;
        distintas++;
        total += ap[k].cantidad;
        printf("%-15s %-20s %-8d ", TABLA[k].c, TABLA[k].es, ap[k].cantidad);
        int mostrar = ap[k].cantidad < 10 ? ap[k].cantidad : 10;
        for (int j = 0; j < mostrar; j++)
            printf("%d%s", ap[k].lineas[j], j < mostrar - 1 ? ", " : "");
        if (ap[k].cantidad > mostrar)
            printf(", ...");
        printf("\n");
    }

    if (total == 0)
        printf("No se encontraron palabras reservadas.\n");
    else
        printf("\nPalabras reservadas distintas: %d | Apariciones totales: %d\n",
               distintas, total);
    printf("Codigo traducido guardado en: traducido.txt\n");

    /* Liberar memoria */
    for (int k = 0; k < NUM_PAL; k++)
        free(ap[k].lineas);
    free(ap);
    free(texto);
    return 0;
}