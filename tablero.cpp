#include "tablero.h"
#include "bits.h"

unsigned char* memoria = nullptr;
int filas = 0;
int columnas = 0;
int bytes = 0;

// convierte una coordenada del tablero en una posicion seguida
int indice_de(int fila, int columna, int cantidad_columnas)
{
    return fila * cantidad_columnas + columna;
}

void reconstruir(int nuevas_filas, int nuevas_columnas, int posicion_fila, int posicion_columna, bool insertar_fila, bool insertar_columna)
{
    // se arma otro bloque para no perder las fichas al cambiar las dimensiones
    int nuevas_posiciones = nuevas_filas * nuevas_columnas;
    int nuevos_bytes = bytes_necesarios(nuevas_posiciones);
    unsigned char* nueva_memoria = new unsigned char[nuevos_bytes];
    limpiar_bytes(nueva_memoria, nuevos_bytes);

    for (int fila_nueva = 0; fila_nueva < nuevas_filas; ++fila_nueva) {
        for (int columna_nueva = 0; columna_nueva < nuevas_columnas; ++columna_nueva) {
            int fila_anterior = fila_nueva;
            int columna_anterior = columna_nueva;

            if (insertar_fila && fila_nueva > posicion_fila) {
                fila_anterior = fila_nueva - 1;
            }
            if (!insertar_fila && nuevas_filas < filas && fila_nueva >= posicion_fila) {
                fila_anterior = fila_nueva + 1;
            }
            if (insertar_columna && columna_nueva > posicion_columna) {
                columna_anterior = columna_nueva - 1;
            }
            if (!insertar_columna && nuevas_columnas < columnas && columna_nueva >= posicion_columna) {
                columna_anterior = columna_nueva + 1;
            }

            bool fila_valida = fila_anterior >= 0 && fila_anterior < filas;
            bool columna_valida = columna_anterior >= 0 && columna_anterior < columnas;
            bool es_nueva_fila = insertar_fila && fila_nueva == posicion_fila;
            bool es_nueva_columna = insertar_columna && columna_nueva == posicion_columna;

            if (fila_valida && columna_valida && !es_nueva_fila && !es_nueva_columna) {
                unsigned char ficha = leer_ficha(memoria, indice_de(fila_anterior, columna_anterior, columnas));
                escribir_ficha(nueva_memoria, indice_de(fila_nueva, columna_nueva, nuevas_columnas), ficha);
            }
        }
    }

    // al crecer siempre hace falta espacio; al reducir se revisa el 65 por ciento
    bool necesita_nuevo_bloque = nuevos_bytes > bytes
        || nuevos_bytes * 100 < bytes * 65;

    if (necesita_nuevo_bloque) {
        delete[] memoria;
        memoria = nueva_memoria;
        bytes = nuevos_bytes;
    } else {
        // se conserva el bloque cuando todavia tiene espacio suficiente
        limpiar_bytes(memoria, bytes);
        for (int indice = 0; indice < nuevas_posiciones; ++indice) {
            unsigned char ficha = leer_ficha(nueva_memoria, indice);
            escribir_ficha(memoria, indice, ficha);
        }
        delete[] nueva_memoria;
    }

    filas = nuevas_filas;
    columnas = nuevas_columnas;
}
void crear_tablero(int nuevas_filas, int nuevas_columnas)
{
    // la reserva inicial solo tiene el tamano que el usuario pidio
    destruir_tablero();
    filas = nuevas_filas;
    columnas = nuevas_columnas;
    bytes = bytes_necesarios(filas * columnas);
    memoria = new unsigned char[bytes];
    limpiar_bytes(memoria, bytes);
}

void destruir_tablero()
{
    // toda memoria que se pide aqui se libera en este mismo modulo
    delete[] memoria;
    memoria = nullptr;
    filas = 0;
    columnas = 0;
    bytes = 0;
}

int obtener_filas()
{
    return filas;
}

int obtener_columnas()
{
    return columnas;
}

int obtener_bytes()
{
    return bytes;
}

unsigned char* obtener_memoria()
{
    return memoria;
}

unsigned char obtener_ficha(int fila, int columna)
{
    // bits se encarga de buscar los tres bits de esta posicion
    return leer_ficha(memoria, indice_de(fila, columna, columnas));
}

void colocar_ficha(int fila, int columna, unsigned char ficha)
{
    escribir_ficha(memoria, indice_de(fila, columna, columnas), ficha);
}

void agregar_fila(int posicion)
{
    reconstruir(filas + 1, columnas, posicion, 0, true, false);
}

void eliminar_fila(int posicion)
{
    reconstruir(filas - 1, columnas, posicion, 0, false, false);
}

void agregar_columna(int posicion)
{
    reconstruir(filas, columnas + 1, 0, posicion, false, true);
}

void eliminar_columna(int posicion)
{
    reconstruir(filas, columnas - 1, 0, posicion, false, false);
}
