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
    // los numeros de arriba ayudan a ubicar una columna sin adivinar la posicion
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

void pedir_eliminacion()
{
    int fila;
    int columna;

    // por ahora se elimina una sola ficha para probar el recorrido completo
    std::cout << "fila de la ficha que desea eliminar: ";
    std::cin >> fila;
    std::cout << "columna de la ficha que desea eliminar: ";
    std::cin >> columna;

    eliminar_ficha(fila, columna);
}

void mostrar_interfaz()
{
    std::cout << "sweet crush - desafio I\n"; // nombre del juego
    std::cout << "proyecto iniciado correctamente\n"; // inicio basico

    iniciar_juego(2, 3);
    mostrar_bytes();
    mostrar_tablero();
    pedir_eliminacion();
    mostrar_bytes();
    mostrar_tablero();
    destruir_tablero();
}
