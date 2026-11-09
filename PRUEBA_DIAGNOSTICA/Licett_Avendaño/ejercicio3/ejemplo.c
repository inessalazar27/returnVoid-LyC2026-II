#include <stdio.h>

/* while y return dentro de un comentario no se traducen */
typedef struct {
    int id;
    unsigned int edad;
} Persona;

static const int LIMITE = 3;

int sumar(int a, int b)
{
    return a + b;   // return en comentario de linea
}

int main(void)
{
    int whileX = 0, retorno_for = 5;      /* nombres que contienen palabras reservadas */
    char letra = 'a';
    Persona p = {1, 20};

    printf("while y return en un texto no cambian\n");
    printf("sizeof(int) = %d, comilla \" y for\n", (int)sizeof(int));

    for (int i = 0; i < LIMITE; i++) {
        if (i == 1) {
            continue;
        } else {
            whileX = sumar(whileX, i);
        }
    }

    while (retorno_for > 0) {
        retorno_for--;
    }

    switch (letra) {
        case 'a':
            break;
        default:
            break;
    }

    do {
        p.edad++;
    } while (p.edad < 0x1F);

    return 0;
}