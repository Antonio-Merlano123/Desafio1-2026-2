#ifndef tablero_h
#define tablero_h

void crear_tablero(int filas, int columnas);
void destruir_tablero();
int obtener_filas();
int obtener_columnas();
int obtener_bytes();
unsigned char* obtener_memoria();
unsigned char obtener_ficha(int fila, int columna);
void colocar_ficha(int fila, int columna, unsigned char ficha);
void agregar_fila(int posicion);
void eliminar_fila(int posicion);
void agregar_columna(int posicion);
void eliminar_columna(int posicion);

#endif
