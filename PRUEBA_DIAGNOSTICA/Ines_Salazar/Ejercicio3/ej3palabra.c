/* Ejercicio 3: busca palabras reservadas de C en un programa en c
   cargado en memoria dinamica y las traduce al espanol. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOTAL 32

/* Tabla: la palabra en C y su equivalente en espanol (misma posicion) */
const char *ingles[TOTAL] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

const char *espanol[TOTAL] = {
    "automatico", "romper", "caso", "caracter", "constante", "continuar", "por_defecto", "hacer",
    "doble", "sino", "enumeracion", "externo", "flotante", "para", "ir_a", "si",
    "entero", "largo", "registro", "retornar", "corto", "con_signo", "tamano_de", "estatico",
    "estructura", "seleccionar", "definir_tipo", "union", "sin_signo", "vacio", "volatil", "mientras"
};

int main(int argc, char *argv[]) {
    /* Archivo a revisar: el que se pasa por parametro, o este mismo */
  const char *nombre = (argc > 1) ? argv[1] : "C:\\Users\\USUARIO\\OneDrive\\Apps\\Documents\\GitHub\\return-void--2026-II\\PRUEBA_DIAGNOSTICA\\Ines_Salazar\\Ejercicio2\\ej2Pascal.c";

    /* 1. Cargar el archivo en memoria dinamica */
    FILE *f = fopen(nombre, "rb");
    if (f == NULL) {
        printf("No se pudo abrir %s\n", nombre);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);              /* tamano del archivo */
    rewind(f);

    char *texto = malloc(tam + 1);    /* memoria dinamica */
    if (texto == NULL) {
        printf("Sin memoria\n");
        fclose(f);
        return 1;
    }
    size_t leidos = fread(texto, 1, tam, f);
    texto[leidos] = '\0';
    fclose(f);

    /* 2. Recorrer el texto sacando palabra por palabra */
    char palabra[64];
    int largo = 0, encontradas = 0;

    for (size_t i = 0; i <= leidos; i++) {
        char c = texto[i];
        if (isalnum((unsigned char)c) || c == '_') {
            if (largo < 63) palabra[largo++] = c;   /* seguir armando la palabra */
        } else if (largo > 0) {
            palabra[largo] = '\0';                  /* la palabra termino */
            largo = 0;

            /* 3. Buscarla en la tabla de palabras reservadas */
            for (int j = 0; j < TOTAL; j++) {
                if (strcmp(palabra, ingles[j]) == 0) {
                    printf("%-10s -> %s\n", ingles[j], espanol[j]);
                    encontradas++;
                    break;
                }
            }
        }
    }

    printf("\nPalabras reservadas encontradas: %d\n", encontradas);

    free(texto);    /* liberar la memoria */
    return 0;
}