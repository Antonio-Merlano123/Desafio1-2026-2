#include "interfaz.h"
#include "juego.h"
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

void mostrar_tablero()
{
    std::cout << "tablero de fichas:\n";
    std::cout << "    ";
    for (int columna = 0; columna < obtener_columnas(); ++columna) {
        std::cout << columna << ' ';
    }
    std::cout << "\n";

    for (int fila = 0; fila < obtener_filas(); ++fila) {
        std::cout << fila << " | ";
        for (int columna = 0; columna < obtener_columnas(); ++columna) {
            std::cout << static_cast<int>(obtener_ficha(fila, columna)) << ' ';
        }
        std::cout << "\n";
    }
}

void mostrar_interfaz()
{
    std::cout << "sweet crush - desafio I\n"; // nombre del juego
    std::cout << "proyecto iniciado correctamente\n"; // inicio basico

    iniciar_juego(2, 3);
    mostrar_bytes();
    mostrar_tablero();
    destruir_tablero();
}
