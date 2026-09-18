#include "tablero.h"
#include "bits.h"

unsigned char* memoria = nullptr; // bloque de bytes del tablero
int filas = 0; // cantidad de filas
int columnas = 0; // cantidad de columnas
int bytes = 0; // bytes usados en la memoria

// convierte fila y columna en una sola posicion del bloque
int indice_de(int fila, int columna, int cantidad_columnas)
{
    return fila * cantidad_columnas + columna;
}

// funcion para cambiar el tamano del tablero agregando o quitando filas y columnas
void reconstruir(int nuevas_filas, int nuevas_columnas, int posicion_fila, int posicion_columna, bool insertar_fila, bool insertar_columna)
{
    int total_fichas = nuevas_filas * nuevas_columnas;
    int bytes_nuevos = bytes_necesarios(total_fichas);
    
    // creamos un bloque aux en memoria dinamica para copiar las fichas viejas
    unsigned char* aux = new unsigned char[bytes_nuevos];
    limpiar_bytes(aux, bytes_nuevos);

    // recorremos cada casilla del nuevo tablero para calcular su posicion previa
    for (int f = 0; f < nuevas_filas; ++f) {
        for (int c = 0; c < nuevas_columnas; ++c) {
            int orig_f = f;
            int orig_c = c;

            // si insertamos o quitamos fila, se mueve el indice de origen
            if (insertar_fila && f > posicion_fila) {
                orig_f = f - 1;
            } else if (!insertar_fila && nuevas_filas < filas && f >= posicion_fila) {
                orig_f = f + 1;
            }

            // si insertamos o quitamos columna, se mueve el indice de origen
            if (insertar_columna && c > posicion_columna) {
                orig_c = c - 1;
            } else if (!insertar_columna && nuevas_columnas < columnas && c >= posicion_columna) {
                orig_c = c + 1;
            }

            // comprobamos que la casilla de origen fuera valida y no sea la casilla nueva insertada
            if (orig_f >= 0 && orig_f < filas && orig_c >= 0 && orig_c < columnas) {
                bool es_insertada = (insertar_fila && f == posicion_fila) || (insertar_columna && c == posicion_columna);
                if (!es_insertada) {
                    unsigned char ficha = leer_ficha(memoria, indice_de(orig_f, orig_c, columnas));
                    escribir_ficha(aux, indice_de(f, c, nuevas_columnas), ficha);
                }
            }
        }
    }

    // si se necesita mas espacio o la ocupacion cae del 65%, asignamos bloque nuevo en memoria
    if (bytes_nuevos > bytes || bytes_nuevos * 100 < bytes * 65) {
        delete[] memoria;
        memoria = aux;
        bytes = bytes_nuevos;
    } else {
        // si todavia cabe bien en el bloque actual, reutilizamos la memoria existente
        limpiar_bytes(memoria, bytes);
        for (int i = 0; i < total_fichas; ++i) {
            unsigned char ficha = leer_ficha(aux, i);
            escribir_ficha(memoria, i, ficha);
        }
        delete[] aux;
    }

    // guardamos las dimensiones actualizadas
    filas = nuevas_filas;
    columnas = nuevas_columnas;
}
void crear_tablero(int nuevas_filas, int nuevas_columnas)
{
    // si el tablero llega con dimensiones invalidas, se limpia y se deja vacio
    destruir_tablero();

    if (nuevas_filas <= 0 || nuevas_columnas <= 0) {
        filas = 0;
        columnas = 0;
        bytes = 0;
        return;
    }

    filas = nuevas_filas;
    columnas = nuevas_columnas;
    bytes = bytes_necesarios(filas * columnas);

    if (bytes <= 0) {
        filas = 0;
        columnas = 0;
        bytes = 0;
        return;
    }

    memoria = new unsigned char[bytes];
    limpiar_bytes(memoria, bytes);
}

void destruir_tablero()
{
    // libera el bloque y deja todo listo para una nueva partida
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
    // busca el valor de esa ficha desde la memoria compactada
    return leer_ficha(memoria, indice_de(fila, columna, columnas));
}

void colocar_ficha(int fila, int columna, unsigned char ficha)
{
    // escribe una ficha en la posicion pedida
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
