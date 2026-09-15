#ifndef juego_h
#define juego_h

void iniciar_juego(int filas, int columnas);
void eliminar_ficha(int fila, int columna);
void agregar_fila_juego();
void eliminar_fila_juego();
void agregar_columna_juego();
void eliminar_columna_juego();
int obtener_puntaje();
int obtener_movimientos();
int obtener_fichas_eliminadas();
int obtener_combinaciones();
int obtener_cascadas();

#endif
