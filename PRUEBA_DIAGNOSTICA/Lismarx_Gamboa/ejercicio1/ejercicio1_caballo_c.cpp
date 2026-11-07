#include <iostream>
#include <vector>
#include <optional>
#include <chrono>

const int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, h = 8;

struct posicion
{
    int columna;
    int fila;
};

std::vector<posicion> movimiento1 = {
    {b, 4}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 1}, {h, 3}, {g, 5}, {e, 6}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 6}, {b, 7}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {a, 6}, {b, 8}, {d, 7}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 4}, {b, 3}, {a, 1}, {c, 2}, {a, 3}, {c, 4}, {a, 5}, {c, 6}, {a, 7}, {c, 8}, {a, 7}, {c, 6}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {d, 5}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 1}, {h, 3}, {g, 5}, {e, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 6}, {b, 7}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {a, 6}, {b, 8}, {d, 7}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 4}, {b, 3}, {a, 1}, {c, 2}, {a, 3}, {c, 4}, {a, 5}, {c, 6}, {a, 7}, {c, 8}, {a, 7}, {c, 6}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {d, 5}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 1}, {h, 3}, {g, 5}, {e, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 6}, {b, 7}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {a, 6}, {b, 8}, {d, 7}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 4}, {b, 3}, {a, 1}, {c, 2}, {a, 3}, {c, 4}, {a, 5}, {c, 6}, {a, 7}, {c, 8}, {a, 7}, {c, 6}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {d, 5}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 4}, {b, 3}, {a, 1}, {c, 2}, {a, 3}, {c, 4}, {a, 5}, {c, 6}, {a, 7}, {c, 8}, {a, 7}, {c, 6}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {d, 5}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 1}, {h, 3}, {g, 5}, {e, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 6}, {b, 7}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {a, 6}, {b, 8}, {d, 7}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 4}, {b, 3}, {a, 1}, {c, 2}, {a, 3}, {c, 4}, {a, 5}, {c, 6}, {a, 7}, {c, 8}, {a, 7}, {c, 6}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {d, 5}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 1}, {h, 3}, {g, 5}, {e, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 6}, {b, 7}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {a, 6}, {b, 8}, {d, 7}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 4}, {b, 3}, {a, 1}, {c, 2}, {a, 3}, {c, 4}, {a, 5}, {c, 6}, {a, 7}, {c, 8}, {a, 7}, {c, 6}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {d, 5}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 1}, {h, 3}, {g, 5}, {e, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 6}, {b, 7}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {a, 6}, {b, 8}, {d, 7}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}, {c, 3}, {a, 2}, {c, 1}, {e, 2}, {g, 3}, {h, 5}, {g, 7}, {e, 8}, {c, 7}, {a, 8}, {c, 7}, {b, 5}, {d, 4}, {b, 3}, {a, 1}, {c, 2}, {a, 3}, {c, 4}, {a, 5}, {c, 6}, {a, 7}, {c, 8}, {a, 7}, {c, 6}, {a, 5}, {c, 4}, {a, 3}, {c, 2}, {a, 1}, {c, 2}, {b, 4}, {d, 5}, {b, 6}, {a, 8}, {c, 7}, {a, 6}, {c, 5}, {a, 4}};

void horse_movement_validaror(const std::vector<posicion> &movimiento)
{
    std::optional<posicion> movimientoPrevio = std::nullopt;
    for (int i = 0; i < movimiento.size(); i++)
    {
        if (movimientoPrevio)
        {
            if ((movimientoPrevio->columna == movimiento[i].columna + 2) ||
                (movimientoPrevio->columna == movimiento[i].columna - 2) &&
                    ((movimientoPrevio->columna > 0) && (movimientoPrevio->columna < 9)))
            {
                if ((movimientoPrevio->fila == movimiento[i].fila + 1) ||
                    (movimientoPrevio->fila == movimiento[i].fila - 1) &&
                        ((movimientoPrevio->fila > 0) && (movimientoPrevio->fila < 9)))
                {
                    std::cout << "Movimiento valido " << movimiento[i].columna << "," << movimiento[i].fila << '\n';
                    movimientoPrevio = movimiento[i];
                }
                else
                {
                    std::cout << "Movimiento invalido " << movimiento[i].columna << "," << movimiento[i].fila << '\n';
                    return;
                }
            }
            else if ((movimientoPrevio->columna == movimiento[i].columna + 1) ||
                     (movimientoPrevio->columna == movimiento[i].columna - 1) &&
                         ((movimientoPrevio->columna > 0) && (movimientoPrevio->columna < 9)))
            {
                if ((movimientoPrevio->fila == movimiento[i].fila + 2) ||
                    (movimientoPrevio->fila == movimiento[i].fila - 2) &&
                        ((movimientoPrevio->fila > 0) && (movimientoPrevio->fila < 9)))
                {
                    std::cout << "Movimiento valido " << movimiento[i].columna << "," << movimiento[i].fila << '\n';
                    movimientoPrevio = movimiento[i];
                }
                else
                {
                    std::cout << "Movimiento invalido " << movimiento[i].columna << "," << movimiento[i].fila << '\n';
                    return;
                }
            }
            else
            {
                std::cout << "Movimiento invalido " << movimiento[i].columna << "," << movimiento[i].fila << '\n';
                return;
            }
        }
        movimientoPrevio = movimiento[i];
    }
}

int main(int argc, char const *argv[])
{
    auto tiempo_inicio = std::chrono::high_resolution_clock::now();
    horse_movement_validaror(movimiento1);
    auto tiempo_fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracion = tiempo_fin - tiempo_inicio;
    std::cout << "El programa tardo: " << duracion.count() << " ms" << '\n';
    return 0;
}