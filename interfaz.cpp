#include "interfaz.h"
#include "juego.h"
#include "tablero.h"
#include <iostream>

using namespace std;

// muestra los bytes de la memoria en binario
void mostrar_bytes()
{
    unsigned char* memoria = obtener_memoria();

    cout << "bytes del tablero: ";
    for (int i = 0; i < obtener_bytes(); ++i) {
        for (int bit = 7; bit >= 0; --bit) {
            cout << ((memoria[i] >> bit) & 1);
        }
        cout << ' ';
    }
    cout << "\n";
}

// imprime el tablero con numeritos en filas y columnas
void mostrar_tablero()
{
    cout << "\ntablero de fichas:\n    ";
    for (int c = 0; c < obtener_columnas(); ++c) {
        cout << c << ' ';
    }
    cout << "\n";

    for (int f = 0; f < obtener_filas(); ++f) {
        cout << f << " | ";
        for (int c = 0; c < obtener_columnas(); ++c) {
            cout << static_cast<int>(obtener_ficha(f, c)) << ' ';
        }
        cout << "\n";
    }
}

// muestra las estadisticas e indicadores de la partida
void mostrar_estadisticas()
{
    cout << "\n--- estado del juego ---\n";
    cout << "dimensiones actuales: " << obtener_filas() << " x " << obtener_columnas() << "\n";
    cout << "movimientos del usuario: " << obtener_movimientos() << "\n";
    cout << "fichas eliminadas en total: " << obtener_fichas_eliminadas() << "\n";
    cout << "combinaciones detectadas: " << obtener_combinaciones() << "\n";
    cout << "cascadas producidas: " << obtener_cascadas() << "\n";
    cout << "puntuacion obtenida: " << obtener_puntaje() << "\n";
    cout << "bytes de memoria usados: " << obtener_bytes() << "\n";
}

// bucle del menu para pedir la jugada
void mostrar_interfaz()
{
    cout << "=== sweet crush - desafio 1 ===\n\n";

    int f_ini = 0;
    int c_ini = 0;

    cout << "ingrese la cantidad de filas iniciales: ";
    while (!(cin >> f_ini) || f_ini <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "por favor ingrese un numero valido: ";
    }
    cout << "ingrese la cantidad de columnas iniciales: ";
    while (!(cin >> c_ini) || c_ini <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "por favor ingrese un numero valido: ";
    }

    iniciar_juego(f_ini, c_ini);

    int opcion = -1;
    while (opcion != 0) {
        mostrar_bytes();
        mostrar_tablero();
        mostrar_estadisticas();

        cout << "\n--- menu de opciones ---\n";
        cout << "1. eliminar una ficha\n";
        cout << "2. agregar una fila\n";
        cout << "3. eliminar una fila\n";
        cout << "4. agregar una columna\n";
        cout << "5. eliminar una columna\n";
        cout << "0. salir\n";
        cout << "seleccione una opcion: ";
        
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
        }

        // se procesa la opc elegida
        switch (opcion) {
            case 1: {
                int f = 0, c = 0;
                cout << "fila: ";
                if (!(cin >> f)) { cin.clear(); cin.ignore(10000, '\n'); break; }
                cout << "columna: ";
                if (!(cin >> c)) { cin.clear(); cin.ignore(10000, '\n'); break; }
                eliminar_ficha(f, c);
                break;
            }
            case 2:
                agregar_fila_juego();
                break;
            case 3:
                eliminar_fila_juego();
                break;
            case 4:
                agregar_columna_juego();
                break;
            case 5:
                eliminar_columna_juego();
                break;
            case 0:
                cout << "\nsaliendo del juego...\n";
                break;
            default:
                cout << "\nopcion no valida, intente de nuevo.\n";
                break;
        }
    }

    destruir_tablero();
    cout << "gracias por jugar!\n";
}
