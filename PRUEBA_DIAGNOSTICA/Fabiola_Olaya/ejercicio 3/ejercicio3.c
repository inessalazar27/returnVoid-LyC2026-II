#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estructura para el diccionario de las 32 palabras reservadas de ANSI C
typedef struct {
    const char *ingles;
    const char *espanol;
} PalabraReservada;

const PalabraReservada diccionario_c[32] = {
    {"auto", "automatico"}, {"break", "romper"}, {"case", "caso"}, {"char", "caracter"},
    {"const", "constante"}, {"continue", "continuar"}, {"default", "por_defecto"}, {"do", "hacer"},
    {"double", "doble"}, {"else", "sino"}, {"enum", "enumeracion"}, {"extern", "externo"},
    {"float", "flotante"}, {"for", "para"}, {"goto", "ir_a"}, {"if", "si"},
    {"int", "entero"}, {"long", "largo"}, {"register", "registro"}, {"return", "retornar"},
    {"short", "corto"}, {"signed", "con_signo"}, {"sizeof", "tamano_de"}, {"static", "estatico"},
    {"struct", "estructura"}, {"switch", "seleccionar"}, {"typedef", "definir_tipo"}, {"union", "union"},
    {"unsigned", "sin_signo"}, {"void", "vacio"}, {"volatile", "volatil"}, {"while", "mientras"}
};

// Función para crear un archivo C de prueba si no existe
void crear_archivo_prueba(const char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "w");
    if (!f) return;
    fprintf(f,
        "#include <stdio.h>\n\n"
        "// Funcion que calcula una suma\n"
        "int calcular(int limite) {\n"
        "    int suma = 0;\n"
        "    for (int i = 0; i < limite; i++) {\n"
        "        if (i == 5) {\n"
        "            continue;\n"
        "        } else {\n"
        "            suma += i;\n"
        "        }\n"
        "    }\n"
        "    return suma;\n"
        "}\n\n"
        "int main() {\n"
        "    double resultado = (double)calcular(10);\n"
        "    while (resultado > 0) {\n"
        "        break;\n"
        "    }\n"
        "    return 0;\n"
        "}\n"
    );
    fclose(f);
}

// 1. Carga el programa en C en memoria dinámica usando malloc
char* cargar_en_memoria_dinamica(const char *nombre_archivo, long *tamano_out) {
    FILE *f = fopen(nombre_archivo, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long tamano = ftell(f);
    rewind(f);

    // Reserva dinámica exacta en el Heap
    char *buffer = (char*)malloc((tamano + 1) * sizeof(char));
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, tamano, f);
    buffer[tamano] = '\0';
    fclose(f);

    if (tamano_out) *tamano_out = tamano;
    return buffer;
}

// Busca si un token es palabra reservada y devuelve su traducción
const char* buscar_traduccion(const char *palabra, int *indice_encontrado) {
    for (int i = 0; i < 32; i++) {
        if (strcmp(palabra, diccionario_c[i].ingles) == 0) {
            *indice_encontrado = i;
            return diccionario_c[i].espanol;
        }
    }
    *indice_encontrado = -1;
    return NULL;
}

// 2. Analiza, traduce y reconstruye el código usando memoria dinámica (malloc / realloc)
char* traducir_codigo_dinamico(const char *codigo_fuente, int *conteo_palabras) {
    size_t capacidad = strlen(codigo_fuente) * 2 + 1;
    char *resultado = (char*)malloc(capacidad * sizeof(char));
    if (!resultado) return NULL;

    size_t len_res = 0;
    resultado[0] = '\0';
    int i = 0;

    while (codigo_fuente[i] != '\0') {
        // Ignorar cadenas de texto entre comillas "..."
        if (codigo_fuente[i] == '"') {
            resultado[len_res++] = codigo_fuente[i++];
            while (codigo_fuente[i] != '\0' && codigo_fuente[i] != '"') {
                if (codigo_fuente[i] == '\\' && codigo_fuente[i+1] != '\0') {
                    resultado[len_res++] = codigo_fuente[i++];
                }
                resultado[len_res++] = codigo_fuente[i++];
            }
            if (codigo_fuente[i] == '"') resultado[len_res++] = codigo_fuente[i++];
        }
        // Ignorar directivas de preprocesador (#include)
        else if (codigo_fuente[i] == '#') {
            while (codigo_fuente[i] != '\0' && codigo_fuente[i] != '\n') {
                resultado[len_res++] = codigo_fuente[i++];
            }
        }
        // Ignorar comentarios de una línea (//)
        else if (codigo_fuente[i] == '/' && codigo_fuente[i+1] == '/') {
            while (codigo_fuente[i] != '\0' && codigo_fuente[i] != '\n') {
                resultado[len_res++] = codigo_fuente[i++];
            }
        }
        // Identificar palabras y verificar si son reservadas
        else if (isalpha(codigo_fuente[i]) || codigo_fuente[i] == '_') {
            char token[128];
            int k = 0;
            while (isalnum(codigo_fuente[i]) || codigo_fuente[i] == '_') {
                if (k < 127) token[k++] = codigo_fuente[i];
                i++;
            }
            token[k] = '\0';

            int idx = -1;
            const char *traduccion = buscar_traduccion(token, &idx);
            const char *texto_a_copiar = (traduccion != NULL) ? traduccion : token;

            if (idx != -1 && conteo_palabras != NULL) {
                conteo_palabras[idx]++;
            }

            size_t len_agregar = strlen(texto_a_copiar);
            if (len_res + len_agregar + 10 >= capacidad) {
                capacidad *= 2;
                char *temp = (char*)realloc(resultado, capacidad * sizeof(char));
                if (!temp) {
                    free(resultado);
                    return NULL;
                }
                resultado = temp;
            }

            strcpy(&resultado[len_res], texto_a_copiar);
            len_res += len_agregar;
        }
        // Copiar símbolos, espacios y saltos de línea
        else {
            if (len_res + 5 >= capacidad) {
                capacidad *= 2;
                char *temp = (char*)realloc(resultado, capacidad * sizeof(char));
                if (!temp) {
                    free(resultado);
                    return NULL;
                }
                resultado = temp;
            }
            resultado[len_res++] = codigo_fuente[i++];
        }
    }

    resultado[len_res] = '\0';
    return resultado;
}

int main() {
    const char *archivo_entrada = "programa_prueba.c";
    crear_archivo_prueba(archivo_entrada);

    // 1. Cargar programa en memoria dinámica
    long tamano = 0;
    char *codigo_en_memoria = cargar_en_memoria_dinamica(archivo_entrada, &tamano);
    if (!codigo_en_memoria) {
        printf("Error al cargar el archivo en memoria dinamica.\n");
        return 1;
    }

    // Arreglo dinámico para contar apariciones de las 32 palabras reservadas
    int *conteo = (int*)calloc(32, sizeof(int));
    if (!conteo) {
        free(codigo_en_memoria);
        return 1;
    }

    // 2. Traducir código analizando el bloque en memoria dinámica
    char *codigo_traducido = traducir_codigo_dinamico(codigo_en_memoria, conteo);

    // 3. Mostrar resultados por consola
    printf("=== EJERCICIO 3: TRADUCTOR DE PALABRAS RESERVADAS EN C ===\n\n");
    printf("--- CODIGO ORIGINAL CARGADO EN MEMORIA ---\n%s\n", codigo_en_memoria);
    
    printf("--- PALABRAS RESERVADAS ENCONTRADAS ---\n");
    for (int i = 0; i < 32; i++) {
        if (conteo[i] > 0) {
            printf("* '%s' -> '%s' (Apariciones: %d)\n",
                   diccionario_c[i].ingles, diccionario_c[i].espanol, conteo[i]);
        }
    }

    printf("\n--- CODIGO TRADUCIDO AL ESPANOL ---\n%s\n", codigo_traducido);

    // Guardar reporte en archivo de texto
    FILE *f_out = fopen("resultado_ejercicio3_c.txt", "w");
    if (f_out) {
        fprintf(f_out, "=== REPORTE EJERCICIO 3 (LENGUAJE C) ===\n\n");
        fprintf(f_out, "--- 1. CODIGO ORIGINAL ---\n%s\n", codigo_en_memoria);
        fprintf(f_out, "--- 2. PALABRAS TRADUCIDAS ---\n");
        for (int i = 0; i < 32; i++) {
            if (conteo[i] > 0) {
                fprintf(f_out, "* '%s' -> '%s' (Apariciones: %d)\n",
                        diccionario_c[i].ingles, diccionario_c[i].espanol, conteo[i]);
            }
        }
        fprintf(f_out, "\n--- 3. CODIGO TRADUCIDO ---\n%s\n", codigo_traducido);
        fclose(f_out);
        printf("\n[Archivo 'resultado_ejercicio3_c.txt' generado con exito]\n");
    }

    // 4. Liberar toda la memoria dinámica utilizada (Heap)
    free(codigo_en_memoria);
    free(codigo_traducido);
    free(conteo);

    return 0;
}