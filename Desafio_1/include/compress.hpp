#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <cstdint>
#include <cctype>


/**
 * @brief Realiza una rotación de bits a la derecha
 * @param v: Bytes a rotar
 * @param n: numero a rotar
 *
 * @return v rotada a la derecha n bits
 */
uint8_t ror_8 (uint8_t v, unsigned int n);

/**
 * @brief Realiza una rotación de bits a la izquiera
 * @param v: Bytes a rotar
 * @param n: numero a rotar
 *
 * @return v rotada a la izquierda n bits
 */
uint8_t rol_8 (uint8_t v, unsigned int n);

/**
 * @brief Realiza descompresión de RLE
 * @param in: la cadena de entrada comprimida
 * @param len: longitud de la cadena
 *
 * @return out: puntero a la cadena descomprimida.
*/

char* rle_decompress(const uint8_t* in, size_t len);

#endif // COMPRESS_HPP
