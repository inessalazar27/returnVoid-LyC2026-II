#include <stdio.h>

// Funcion que calcula una suma
int calcular(int limite) {
    int suma = 0;
    for (int i = 0; i < limite; i++) {
        if (i == 5) {
            continue;
        } else {
            suma += i;
        }
    }
    return suma;
}

int main() {
    double resultado = (double)calcular(10);
    while (resultado > 0) {
        break;
    }
    return 0;
}
