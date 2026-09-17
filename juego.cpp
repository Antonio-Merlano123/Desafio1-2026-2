#include "juego.h"
#include "tablero.h"

const unsigned char ficha_vacia = 6; // hueco libre del tablero
const unsigned char ficha_marca = 7; // ficha marcada para borrar

unsigned int semilla = 123456789;

int totalpuntos       = 0;
int movimientos       = 0;
int fichas_eliminadas = 0;
int combinaciones     = 0;
int cascadas          = 0;

unsigned char ficha_aleatoria()
{
    // formula lineal clasica para no usar rand ni nada de ansi
    semilla = semilla * 1103515245u + 12345u;
    return static_cast<unsigned char>((semilla >> 16) % 6);
}

void llenar_vacios()
{
    for (int f = 0; f < obtener_filas(); ++f) {
        for (int c = 0; c < obtener_columnas(); ++c) {
            if (obtener_ficha(f, c) == ficha_vacia)
                colocar_ficha(f, c, ficha_aleatoria());
        }
    }
}

// marca las fichas que forman trio horizontal, devuelve true si encontro alguna
bool buscar_horizontal()
{
    bool encontro = false;
    int filas = obtener_filas();
    int cols  = obtener_columnas();

    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < cols - 2; ++c) {
            unsigned char tipo = obtener_ficha(f, c);
            if (tipo < 6 && tipo == obtener_ficha(f, c + 1) && tipo == obtener_ficha(f, c + 2)) {
                colocar_ficha(f, c,     ficha_marca);
                colocar_ficha(f, c + 1, ficha_marca);
                colocar_ficha(f, c + 2, ficha_marca);
                encontro = true;
            }
        }
    }
    return encontro;
}

// marca las fichas que forman trio vertical, devuelve true si encontro alguna
bool buscar_vertical()
{
    bool encontro = false;
    int filas = obtener_filas();
    int cols  = obtener_columnas();

    for (int c = 0; c < cols; ++c) {
        for (int f = 0; f < filas - 2; ++f) {
            unsigned char tipo = obtener_ficha(f, c);
            if (tipo < 6 && tipo == obtener_ficha(f + 1, c) && tipo == obtener_ficha(f + 2, c)) {
                colocar_ficha(f,     c, ficha_marca);
                colocar_ficha(f + 1, c, ficha_marca);
                colocar_ficha(f + 2, c, ficha_marca);
                encontro = true;
            }
        }
    }
    return encontro;
}

bool marcar_combinaciones()
{
    // primero horizontal, luego vertical para marcar cruces en T o L completas
    bool h = buscar_horizontal();
    bool v = buscar_vertical();
    return h || v;
}

// recorre el tablero buscando fichas con marca para ponerlas como hueco libre
int eliminar_marcadas()
{
    int borradas = 0;
    int f_totales = obtener_filas();
    int c_totales = obtener_columnas();

    for (int f = 0; f < f_totales; ++f) {
        for (int c = 0; c < c_totales; ++c) {
            if (obtener_ficha(f, c) == ficha_marca) {
                colocar_ficha(f, c, ficha_vacia);
                ++borradas;
            }
        }
    }
    return borradas;
}

void aplicar_gravedad()
{
    // columna por columna: las fichas reales bajan, los huecos quedan arriba
    for (int c = 0; c < obtener_columnas(); ++c) {
        int dest = obtener_filas() - 1;
        for (int f = obtener_filas() - 1; f >= 0; --f) {
            unsigned char tipo = obtener_ficha(f, c);
            if (tipo != ficha_vacia) {
                colocar_ficha(dest, c, tipo);
                --dest;
            }
        }
        while (dest >= 0)
            colocar_ficha(dest--, c, ficha_vacia);
    }
}

void resolver_cascadas()
{
    cascadas = 0;
    while (marcar_combinaciones()) {
        int borradas = eliminar_marcadas();
        ++combinaciones;
        fichas_eliminadas += borradas;
        totalpuntos += borradas * (cascadas + 1); // cada cascada extra vale mas
        aplicar_gravedad();
        llenar_vacios();
        ++cascadas;
    }
    if (cascadas > 0)
        --cascadas; // el ultimo ciclo no genera caida real, se descuenta
}

void rellenar_despues_de_cambio()
{
    llenar_vacios();
    resolver_cascadas();
}

void iniciar_juego(int filas, int columnas)
{
    crear_tablero(filas, columnas);
    totalpuntos       = 0;
    movimientos       = 0;
    fichas_eliminadas = 0;
    combinaciones     = 0;
    cascadas          = 0;

    for (int f = 0; f < filas; ++f)
        for (int c = 0; c < columnas; ++c)
            colocar_ficha(f, c, ficha_aleatoria());
}

void eliminar_ficha(int fila, int columna)
{
    if (fila < 0 || fila >= obtener_filas() || columna < 0 || columna >= obtener_columnas())
        return;

    colocar_ficha(fila, columna, ficha_vacia);
    aplicar_gravedad();
    llenar_vacios();
    ++movimientos;
    resolver_cascadas();
}

void agregar_fila_juego()
{
    agregar_fila(obtener_filas() / 2);
    rellenar_despues_de_cambio();
}

void eliminar_fila_juego()
{
    if (obtener_filas() > 1) {
        eliminar_fila(obtener_filas() / 2);
        rellenar_despues_de_cambio();
    }
}

void agregar_columna_juego()
{
    agregar_columna(obtener_columnas() / 2);
    rellenar_despues_de_cambio();
}

void eliminar_columna_juego()
{
    if (obtener_columnas() > 1) {
        eliminar_columna(obtener_columnas() / 2);
        rellenar_despues_de_cambio();
    }
}

int obtener_puntaje()           { return totalpuntos; }
int obtener_movimientos()       { return movimientos; }
int obtener_fichas_eliminadas() { return fichas_eliminadas; }
int obtener_combinaciones()     { return combinaciones; }
int obtener_cascadas()          { return cascadas; }
