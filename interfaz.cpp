#include "interfaz.h"
#include "tablero.h"
#include <iostream>

void mostrar_bytes()
{
    unsigned char* memoria = obtener_memoria();

    std::cout << "bytes del tablero: ";
    for (int indice = 0; indice < obtener_bytes(); ++indice) {
        for (int bit = 7; bit >= 0; --bit) {
            std::cout << ((memoria[indice] >> bit) & 1);
        }
        std::cout << ' ';
    }
    std::cout << "\n";
}

void mostrar_interfaz()
{
    std::cout << "sweet crush - desafio I\n"; // nombre del juego
    std::cout << "proyecto iniciado correctamente\n"; // inicio basico

    crear_tablero(1, 1);
    mostrar_bytes();
    destruir_tablero();
}
