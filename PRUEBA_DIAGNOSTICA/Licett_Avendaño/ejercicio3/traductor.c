/* Ejercicio 3 - Traductor de palabras reservadas de C al español
   Carga un programa en C en memoria dinamica busca las palabras
   reservadas y las reemplaza por su equivalente en español */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOTAL_PALABRAS 35

struct palabra {
    const char *ingles;
    const char *espanol;
};

/* 32 palabras reservadas de C estandar + 3 de C99
   La traduccion va sin espacios ni tildes para que siga siendo una sola palabra. */
struct palabra tabla[TOTAL_PALABRAS] = {
    {"auto", "automatico"},   {"break", "romper"},       {"case", "caso"},
    {"char", "caracter"},     {"const", "constante"},    {"continue", "continuar"},
    {"default", "defecto"},   {"do", "hacer"},           {"double", "doble"},
    {"else", "sino"},         {"enum", "enumeracion"},   {"extern", "externo"},
    {"float", "flotante"},    {"for", "para"},           {"goto", "ir_a"},
    {"if", "si"},             {"inline", "enlinea"},     {"int", "entero"},
    {"long", "largo"},        {"register", "registro"},  {"restrict", "restringido"},
    {"return", "retornar"},   {"short", "corto"},        {"signed", "con_signo"},
    {"sizeof", "tamano_de"},  {"static", "estatico"},    {"struct", "estructura"},
    {"switch", "segun"},      {"typedef", "definir_tipo"}, {"union", "union"},
    {"unsigned", "sin_signo"}, {"void", "vacio"},        {"volatile", "volatil"},
    {"while", "mientras"},    {"_Bool", "_Booleano"}
};

/* el texto de salida crece con realloc segun haga falta */
struct salida {
    char *datos;
    size_t usado;
    size_t capacidad;
};


char *cargarArchivo(const char *nombre, long *tamano)
{
    FILE *f = fopen(nombre, "rb");
    if (f == NULL) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long largo = ftell(f);
    rewind(f);

    char *texto = malloc(largo + 1);
    if (texto == NULL) {
        fclose(f);
        return NULL;
    }
    size_t leidos = fread(texto, 1, largo, f);
    texto[leidos] = '\0';
    *tamano = (long)leidos;
    fclose(f);
    return texto;
}


/* Busca la palabra en la tabl, una por una y evuelve la posicion o -1 */
int buscarPalabra(const char *p)
{
    for (int i = 0; i < TOTAL_PALABRAS; i++) {
        if (strcmp(p, tabla[i].ingles) == 0) {
            return i;
        }
    }
    return -1;
}


void agregar(struct salida *s, const char *txt, size_t largo)
{
    while (s->usado + largo + 1 > s->capacidad) {
        size_t nueva = s->capacidad * 2;
        char *aux = realloc(s->datos, nueva);
        if (aux == NULL) {
            printf("No hay memoria suficiente\n");
            exit(1);
        }
        s->datos = aux;
        s->capacidad = nueva;
    }
    memcpy(s->datos + s->usado, txt, largo);
    s->usado = s->usado + largo;
    s->datos[s->usado] = '\0';
}


/* Recorre el texto, los comentarios y lo que esta entre comillas
   se copian igual "while" es solo textonno una palabra reservada */
void traducir(const char *texto, struct salida *s, int *veces)
{
    size_t n = strlen(texto);
    size_t i = 0;

    while (i < n) {
        char c = texto[i];
        size_t ini = i;

        if (c == '/' && texto[i + 1] == '/') {
            /* comentario de una linea */
            while (i < n && texto[i] != '\n') {
                i++;
            }
            agregar(s, texto + ini, i - ini);
        }
        else if (c == '/' && texto[i + 1] == '*') {
            /* comentario de bloque */
            i = i + 2;
            while (i < n && !(texto[i] == '*' && texto[i + 1] == '/')) {
                i++;
            }
            if (i < n) {
                i = i + 2;
            }
            agregar(s, texto + ini, i - ini);
        }
        else if (c == '"' || c == '\'') {
            /* cadena o caracter: se salta hasta la comilla que cierra */
            char comilla = c;
            i++;
            while (i < n && texto[i] != comilla) {
                if (texto[i] == '\\') {
                    i++;            /* el caracter que sigue a \ no cierra nada */
                }
                i++;
            }
            if (i > n) {
                i = n;
            }
            if (i < n) {
                i++;                /* la comilla de cierre */
            }
            agregar(s, texto + ini, i - ini);
        }
        else if (isdigit((unsigned char)c)) {
            /* numeros como 0x1F o 10L no son identificadores */
            while (i < n && (isalnum((unsigned char)texto[i]) || texto[i] == '.')) {
                i++;
            }
            agregar(s, texto + ini, i - ini);
        }
        else if (isalpha((unsigned char)c) || c == '_') {
            /* identificador o palabra reservada */
            while (i < n && (isalnum((unsigned char)texto[i]) || texto[i] == '_')) {
                i++;
            }
            size_t largo = i - ini;
            char palabra[64];
            int pos = -1;

            if (largo < sizeof(palabra)) {
                memcpy(palabra, texto + ini, largo);
                palabra[largo] = '\0';
                pos = buscarPalabra(palabra);
            }
            if (pos >= 0) {
                veces[pos]++;
                agregar(s, tabla[pos].espanol, strlen(tabla[pos].espanol));
            } else {
                agregar(s, texto + ini, largo);
            }
        }
        else {
            agregar(s, &c, 1);
            i++;
        }
    }
}


int main(int argc, char *argv[])
{
    const char *nombre = "ejemplo.c";
    if (argc > 1) {
        nombre = argv[1];
    }

    long tamano = 0;
    char *programa = cargarArchivo(nombre, &tamano);
    if (programa == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre);
        return 1;
    }
    printf("Archivo: %s (%ld caracteres cargados en memoria)\n", nombre, tamano);

    struct salida resultado;
    resultado.capacidad = tamano + 16;
    resultado.usado = 0;
    resultado.datos = malloc(resultado.capacidad);
    resultado.datos[0] = '\0';

    int *veces = calloc(TOTAL_PALABRAS, sizeof(int));

    traducir(programa, &resultado, veces);

    /* informe */
    int distintas = 0;
    int total = 0;
    printf("\nPalabras reservadas encontradas:\n");
    printf("%-12s %-15s %s\n", "C", "Espanol", "Veces");
    for (int i = 0; i < TOTAL_PALABRAS; i++) {
        if (veces[i] > 0) {
            printf("%-12s %-15s %d\n", tabla[i].ingles, tabla[i].espanol, veces[i]);
            distintas++;
            total = total + veces[i];
        }
    }
    if (total == 0) {
        printf("(ninguna)\n");
    } else {
        printf("Total: %d palabras distintas, %d apariciones\n", distintas, total);
    }

    printf("\n===== PROGRAMA TRADUCIDO =====\n%s\n", resultado.datos);

    FILE *salida = fopen("traducido.txt", "w");
    if (salida != NULL) {
        fputs(resultado.datos, salida);
        fclose(salida);
        printf("(guardado tambien en traducido.txt)\n");
    }

    free(programa);
    free(resultado.datos);
    free(veces);
    return 0;
}