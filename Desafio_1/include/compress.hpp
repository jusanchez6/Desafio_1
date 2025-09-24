/**
 * @file compress.hpp
 * @brief Archivo de cabecera para las funciones de descompresión y operaciones a nivel de bit
 *
 * Este modulo contiene:
 * ror_8: Rotación de bits a la derecha
 * rol_8: Rotación de bits a la izquierda
 * rle_decompress: Función para la descompresión por medio de RLE
 * LZ78_decompress: Función para la descompresión por medio de LZ78
 *
 * @authors Julián Sánchez
 *          Jaider Bedoya
 * 
 * @date 22/09/2025
 * @version 1.03
 */

#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include <iostream>
#include <cstdint>
#include <cctype>
#include <cstring>

using namespace std;


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
 * @brief   Descomprime una cadena usando Run-Length Encoding (RLE).
 *
 * @param   in   Puntero a los datos comprimidos (ej. "5A3B2C").
 * @param   len  Longitud de la entrada en bytes.
 *
 * @return  Un puntero a la cadena descomprimida (char*), o nullptr en caso de error.
 *          El usuario es responsable de liberar la memoria con `delete[]`.
 *
 * @note    El formato esperado es: <count><symbol>, repetido varias veces.
 *          Ejemplo: "4A2B1C" → "AAAABB C".
 *
 * Ejemplo de uso:
 * @code
 * const uint8_t data[] = "5A3B2C";
 * char* result = rle_decompress(data, strlen((char*)data));
 * if (result) {
 *     printf("Descomprimido: %s\n", result); // "AAAAABBBCC"
 *     delete[] result;
 * }
 * @endcode
 */
char *rle_decompress(const uint8_t *in, size_t len);


/**
 * @brief   Descomprime datos usando el algoritmo LZ78.
 *
 * @param   in   Puntero al arreglo de bytes comprimidos.
 * @param   len  Longitud de la entrada en bytes.
 *
 * @return  Puntero a la cadena descomprimida (char*).
 *          El usuario es responsable de liberar esta memoria con `delete[]`.
 *          Retorna nullptr en caso de error en la entrada.
 *
 * @note    La función asigna memoria dinámica para la salida y para el diccionario.
 *          En caso de error, se libera toda la memoria reservada.
 *
 * Ejemplo de uso:
 * @code
 * const uint8_t data[] = "0A1B2C";
 * char* result = lz78_decompress(data, strlen((char*)data));
 * if (result) {
 *     printf("Descomprimido: %s\n", result);
 *     delete[] result;
 * }
 * @endcode
 */
char *lz78_decompress(const uint8_t *in, size_t len);



#endif // COMPRESS_HPP
