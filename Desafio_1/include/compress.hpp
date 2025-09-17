#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <cstdint>


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

#endif // COMPRESS_HPP
