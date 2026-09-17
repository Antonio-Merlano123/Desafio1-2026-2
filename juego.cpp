#include "juego.h"
#include "tablero.h"

const unsigned char ficha_vacia = 6; // hueco libre del tablero
const unsigned char ficha_marca = 7; // ficha marcada para borrar

unsigned int semilla = 123456789; // valor base para generar fichas

int totalpuntos = 0; // puntaje total
int movimientos = 0; // veces que se mueve o borra
int fichas_eliminadas = 0; // fichas borradas
int combinaciones = 0; // grupos resueltos
int cascadas = 0; // secuencias de combos

unsigned char ficha_aleatoria()
{
    semilla = semilla * 1103515245u + 12345u; // cambia la semilla
    return static_cast<unsigned char>((semilla >> 16) % 6); // da un valor entre 0 y 5
}

void llenar_vacios()
{
    for (int fila = 0; fila < obtener_filas(); ++fila) { // recorre filas
        for (int columna = 0; columna < obtener_columnas(); ++columna) { // recorre columnas
            if (obtener_ficha(fila, columna) == ficha_vacia) { // hay un hueco
                colocar_ficha(fila, columna, ficha_aleatoria()); // lo rellena con una ficha nueva
            }
        }
    }
}

bool marcar_combinaciones()
{
    bool encontro_combinacion = false; // dice si hubo combo
    int filas = obtener_filas();
    int columnas = obtener_columnas();

    for (int fila = 0; fila < filas; ++fila) {
        for (int columna = 0; columna < columnas - 2; ++columna) {
            unsigned char ficha = obtener_ficha(fila, columna);
            if (ficha < 6 && ficha == obtener_ficha(fila, columna + 1) && ficha == obtener_ficha(fila, columna + 2)) {
                colocar_ficha(fila, columna, ficha_marca); // marca 1
                colocar_ficha(fila, columna + 1, ficha_marca); // marca 2
                colocar_ficha(fila, columna + 2, ficha_marca); // marca 3
                encontro_combinacion = true;
            }
        }
    }

    for (int columna = 0; columna < columnas; ++columna) {
        for (int fila = 0; fila < filas - 2; ++fila) {
            unsigned char ficha = obtener_ficha(fila, columna);
            if (ficha < 6 && ficha == obtener_ficha(fila + 1, columna) && ficha == obtener_ficha(fila + 2, columna)) {
                colocar_ficha(fila, columna, ficha_marca); // marca arriba
                colocar_ficha(fila + 1, columna, ficha_marca); // marca medio
                colocar_ficha(fila + 2, columna, ficha_marca); // marca abajo
                encontro_combinacion = true;
            }
        }
    }

    return encontro_combinacion;
}

int eliminar_marcadas()
{
    int eliminadas = 0; // cuantas fichas se borran
    for (int fila = 0; fila < obtener_filas(); ++fila) {
        for (int columna = 0; columna < obtener_columnas(); ++columna) {
            if (obtener_ficha(fila, columna) == ficha_marca) { // estaba marcada
                colocar_ficha(fila, columna, ficha_vacia); // la convierto en hueco
                ++eliminadas; // cuenta una mas
            }
        }
    }
    return eliminadas;
}

void aplicar_gravedad()
{
    for (int columna = 0; columna < obtener_columnas(); ++columna) { // revisa cada columna
        int fila_destino = obtener_filas() - 1; // empieza abajo
        for (int fila = obtener_filas() - 1; fila >= 0; --fila) {
            unsigned char ficha = obtener_ficha(fila, columna);
            if (ficha != ficha_vacia) { // hay una ficha real
                colocar_ficha(fila_destino, columna, ficha); // la baja
                --fila_destino; // sigue bajando
            }
        }
        while (fila_destino >= 0) { // deja huecos arriba
            colocar_ficha(fila_destino, columna, ficha_vacia);
            --fila_destino;
        }
    }
}

void resolver_cascadas()
{
    cascadas = 0; // reinicia la cuenta de encadenados
    while (marcar_combinaciones()) { // busca combos
        int eliminadas = eliminar_marcadas(); // borra marcas
        ++combinaciones; // suma un combo resuelto
        fichas_eliminadas += eliminadas; // cuenta fichas borradas
        totalpuntos += eliminadas * (cascadas + 1); // puntaje por cadena
        aplicar_gravedad(); // las fichas caen
        llenar_vacios(); // se rellenan huecos
        ++cascadas; // sube el contador de cascada
    }
    if (cascadas > 0) {
        --cascadas; // deja la cuenta final en el valor que se usa en pantalla
    }
}

void rellenar_despues_de_cambio()
{
    llenar_vacios(); // rellena huecos
    resolver_cascadas(); // busca combos que queden al mover cosas
}

void iniciar_juego(int filas, int columnas)
{
    crear_tablero(filas, columnas); // crea el tablero
    totalpuntos = 0; // reinicia score
    movimientos = 0; // reinicia movimiento
    fichas_eliminadas = 0; // reinicia conteo
    combinaciones = 0; // reinicia combos
    cascadas = 0; // reinicia cascadas
    for (int fila = 0; fila < filas; ++fila) {
        for (int columna = 0; columna < columnas; ++columna) {
            colocar_ficha(fila, columna, ficha_aleatoria()); // llena con fichas
        }
    }
}

void eliminar_ficha(int fila, int columna)
{
    if (fila < 0 || fila >= obtener_filas() || columna < 0 || columna >= obtener_columnas()) {
        return; // fuera del tablero
    }
    colocar_ficha(fila, columna, ficha_vacia); // borra esa pieza
    aplicar_gravedad(); // baja las demas
    llenar_vacios(); // rellena huecos
    ++movimientos; // cuenta un movimiento
    resolver_cascadas(); // comprueba combos
}

void agregar_fila_juego()
{
    agregar_fila(obtener_filas() / 2); // agrega una fila al medio
    rellenar_despues_de_cambio(); // revisa tablero nuevo
}

void eliminar_fila_juego()
{
    if (obtener_filas() > 1) { // tiene que quedar al menos una
        eliminar_fila(obtener_filas() / 2); // quita fila central
        rellenar_despues_de_cambio();
    }
}

void agregar_columna_juego()
{
    agregar_columna(obtener_columnas() / 2); // agrega columna central
    rellenar_despues_de_cambio();
}

void eliminar_columna_juego()
{
    if (obtener_columnas() > 1) { // al menos una columna queda
        eliminar_columna(obtener_columnas() / 2); // quita columna media
        rellenar_despues_de_cambio();
    }
}

int obtener_puntaje()
{
    return totalpuntos;
}

int obtener_movimientos()
{
    return movimientos;
}

int obtener_fichas_eliminadas()
{
    return fichas_eliminadas;
}

int obtener_combinaciones()
{
    return combinaciones;
}

int obtener_cascadas()
{
    return cascadas;
}
