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
    // la posicion de cada ficha se cuenta desde el bit menos significativo
    int bit_inicial = indice_ficha * 3;
    int indice_byte = bit_inicial / 8;
    int desplazamiento = bit_inicial % 8;

    if (desplazamiento <= 5) {
        // en este caso los tres bits caben completos en un solo byte
        unsigned char byte_actual = memoria[indice_byte];
        return static_cast<unsigned char>((byte_actual >> desplazamiento) & 7);
    }

    // aqui la ficha cruza al byte siguiente y toca unir las dos partes
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
