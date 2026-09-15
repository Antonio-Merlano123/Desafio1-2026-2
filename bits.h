#ifndef bits_h
#define bits_h

int bytes_necesarios(int cantidad_posiciones);
void limpiar_bytes(unsigned char* memoria, int cantidad_bytes);
unsigned char leer_ficha(const unsigned char* memoria, int indice_ficha);
void escribir_ficha(unsigned char* memoria, int indice_ficha, unsigned char valor);

#endif
