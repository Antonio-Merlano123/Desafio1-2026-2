#include "bits.h"

int bytes_necesarios(int cantidad_posiciones)
{
    // si no hay fichas, tampoco se necesita memoria
    if (cantidad_posiciones <= 0) {
        return 0;
    }

    // cada ficha ocupa tres bits y al final se redondea al byte siguiente
    int cantidad_bits = cantidad_posiciones * 3;
    return (cantidad_bits + 7) / 8;
}

void limpiar_bytes(unsigned char* memoria, int cantidad_bytes)
{
    // si no hay bloque o bytes, no hay nada que limpiar
    if (memoria == nullptr || cantidad_bytes <= 0) {
        return;
    }

    // el bloque empieza limpio para no dejar datos viejos en el tablero
    for (int indice = 0; indice < cantidad_bytes; ++indice) {
        memoria[indice] = 0;
    }
}

unsigned char leer_ficha(const unsigned char* memoria, int indice_ficha)
{
    // esta funcion lee una ficha compactada en 3 bits.
    // cada ficha ocupa 3 posiciones de bits en el bloque continuo.
    // cuando una ficha cruza el borde de un byte, se juntan dos partes
    // para leer el valor completo sin tocar el resto del bloque.
    if (memoria == nullptr || indice_ficha < 0) {
        return 0;
    }

    int bit_inicial = indice_ficha * 3;
    int indice_byte = bit_inicial / 8;
    int desplazamiento = bit_inicial % 8;

    // si la ficha queda completa dentro de un solo byte, solo hace falta mover el byte
    // hacia la derecha y quedarnos con los 3 bits de la mascara 00000111.
    if (desplazamiento <= 5) {
        unsigned char byte_actual = memoria[indice_byte];
        return static_cast<unsigned char>((byte_actual >> desplazamiento) & 7);
    }

    // si la ficha cruza al siguiente byte, se recogen dos trozos:
    // la parte que queda en el byte actual y la parte que empieza en el byte siguiente.
    // luego se juntan y se limpia la parte extra con una mascara final.
    unsigned char parte_baja = static_cast<unsigned char>(memoria[indice_byte] >> desplazamiento);
    unsigned char parte_alta = static_cast<unsigned char>(memoria[indice_byte + 1] << (8 - desplazamiento));
    return static_cast<unsigned char>((parte_baja | parte_alta) & 7);
}

void escribir_ficha(unsigned char* memoria, int indice_ficha, unsigned char valor)
{
    int bit_inicial = indice_ficha * 3;
    int indice_byte = bit_inicial / 8;
    int desplazamiento = bit_inicial % 8;
    unsigned char valor_limpio = static_cast<unsigned char>(valor & 7);

    if (desplazamiento <= 5) {
        // primero se borran solo los tres bits de la ficha
        unsigned char mascara = static_cast<unsigned char>(7 << desplazamiento);
        memoria[indice_byte] = static_cast<unsigned char>(memoria[indice_byte] & static_cast<unsigned char>(~mascara));
        memoria[indice_byte] = static_cast<unsigned char>(memoria[indice_byte] | static_cast<unsigned char>(valor_limpio << desplazamiento));
        return;
    }

    // la ficha esta partida, por eso se usan dos mascaras distintas
    int bits_primer_byte = 8 - desplazamiento;
    int bits_segundo_byte = 3 - bits_primer_byte;
    unsigned char mascara_primer_byte = static_cast<unsigned char>((1 << bits_primer_byte) - 1);
    unsigned char mascara_segundo_byte = static_cast<unsigned char>((1 << bits_segundo_byte) - 1);

    memoria[indice_byte] = static_cast<unsigned char>(memoria[indice_byte] & static_cast<unsigned char>(~(mascara_primer_byte << desplazamiento)));
    memoria[indice_byte] = static_cast<unsigned char>(memoria[indice_byte] | ((valor_limpio & mascara_primer_byte) << desplazamiento));

    memoria[indice_byte + 1] = static_cast<unsigned char>(memoria[indice_byte + 1] & static_cast<unsigned char>(~mascara_segundo_byte));
    memoria[indice_byte + 1] = static_cast<unsigned char>(memoria[indice_byte + 1] | ((valor_limpio >> bits_primer_byte) & mascara_segundo_byte));
}
