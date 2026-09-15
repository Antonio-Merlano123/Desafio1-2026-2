#include "juego.h"
#include "tablero.h"

const unsigned char ficha_vacia = 6;
const unsigned char ficha_marca = 7;
unsigned int semilla = 123456789;
int totalpuntos = 0;
int movimientos = 0;
int fichas_eliminadas = 0;
int combinaciones = 0;
int cascadas = 0;

unsigned char ficha_aleatoria()
{
    // se cambia la semilla para obtener un valor nuevo entre cero y cinco
    semilla = semilla * 1103515245u + 12345u;
    return static_cast<unsigned char>((semilla >> 16) % 6);
}

void llenar_vacios()
{
    // solo se llenan los espacios que quedaron despues de una eliminacion
    for (int fila = 0; fila < obtener_filas(); ++fila) {
        for (int columna = 0; columna < obtener_columnas(); ++columna) {
            if (obtener_ficha(fila, columna) == ficha_vacia) {
                colocar_ficha(fila, columna, ficha_aleatoria());
            }
        }
    }
}

bool marcar_combinaciones()
{
    // primero se marcan todas y despues se eliminan para respetar cruces
    bool encontro_combinacion = false;
    int filas = obtener_filas();
    int columnas = obtener_columnas();

    for (int fila = 0; fila < filas; ++fila) {
        for (int columna = 0; columna < columnas - 2; ++columna) {
            unsigned char ficha = obtener_ficha(fila, columna);
            if (ficha < 6 && ficha == obtener_ficha(fila, columna + 1) && ficha == obtener_ficha(fila, columna + 2)) {
                colocar_ficha(fila, columna, ficha_marca);
                colocar_ficha(fila, columna + 1, ficha_marca);
                colocar_ficha(fila, columna + 2, ficha_marca);
                encontro_combinacion = true;
            }
        }
    }

    for (int columna = 0; columna < columnas; ++columna) {
        for (int fila = 0; fila < filas - 2; ++fila) {
            unsigned char ficha = obtener_ficha(fila, columna);
            if (ficha < 6 && ficha == obtener_ficha(fila + 1, columna) && ficha == obtener_ficha(fila + 2, columna)) {
                colocar_ficha(fila, columna, ficha_marca);
                colocar_ficha(fila + 1, columna, ficha_marca);
                colocar_ficha(fila + 2, columna, ficha_marca);
                encontro_combinacion = true;
            }
        }
    }

    return encontro_combinacion;
}

int eliminar_marcadas()
{
    // las marcas pasan a ser espacios vacios en un solo recorrido
    int eliminadas = 0;
    for (int fila = 0; fila < obtener_filas(); ++fila) {
        for (int columna = 0; columna < obtener_columnas(); ++columna) {
            if (obtener_ficha(fila, columna) == ficha_marca) {
                colocar_ficha(fila, columna, ficha_vacia);
                ++eliminadas;
            }
        }
    }
    return eliminadas;
}

void aplicar_gravedad()
{
    // cada columna baja sus fichas hacia el final
    for (int columna = 0; columna < obtener_columnas(); ++columna) {
        int fila_destino = obtener_filas() - 1;
        for (int fila = obtener_filas() - 1; fila >= 0; --fila) {
            unsigned char ficha = obtener_ficha(fila, columna);
            if (ficha != ficha_vacia) {
                colocar_ficha(fila_destino, columna, ficha);
                --fila_destino;
            }
        }
        while (fila_destino >= 0) {
            colocar_ficha(fila_destino, columna, ficha_vacia);
            --fila_destino;
        }
    }
}

void resolver_cascadas()
{
    // una vuelta completa busca, elimina, hace caer y vuelve a llenar
    cascadas = 0;
    while (marcar_combinaciones()) {
        int eliminadas = eliminar_marcadas();
        ++combinaciones;
        fichas_eliminadas += eliminadas;
        totalpuntos += eliminadas * (cascadas + 1);
        aplicar_gravedad();
        llenar_vacios();
        ++cascadas;
    }
    if (cascadas > 0) {
        --cascadas;
    }
}

void rellenar_despues_de_cambio()
{
    llenar_vacios();
    resolver_cascadas();
}
void iniciar_juego(int filas, int columnas)
{
    crear_tablero(filas, columnas);
    totalpuntos = 0;
    movimientos = 0;
    fichas_eliminadas = 0;
    combinaciones = 0;
    cascadas = 0;
    for (int fila = 0; fila < filas; ++fila) {
        for (int columna = 0; columna < columnas; ++columna) {
            colocar_ficha(fila, columna, ficha_aleatoria());
        }
    }
}

void eliminar_ficha(int fila, int columna)
{
    if (fila < 0 || fila >= obtener_filas() || columna < 0 || columna >= obtener_columnas()) {
        return;
    }
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
