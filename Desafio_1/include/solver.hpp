/**
 * @file solver.hpp
 * @brief Archivo de cabecera para la función de busqueda de fuerza bruta
 *
 * Este modulo contiene:
 * finder: Metodo de busqueda por medio de fuerza bruta
 *
 * @authors Julián Sánchez
 *          Jaider Bedoya
 * 
 * @date 22/09/2025
 * @version 1.0
 */

#ifndef SOLVER_HPP
#define SOLVER_HPP

//Librerias Estandar
#include <iostream>
#include <cstdint>
#include <cctype>
#include <cstring>

//Libreria de las operaciones de bit y descompresión
#include <compress.hpp>

// Macro para la comparación del porcentaje de texto imprimible
#define RATIO_MIN 0.3

using namespace std;

/**
 * @brief Intenta descifrar un mensaje encriptado mediante fuerza bruta
 *        probando todas las combinaciones posibles de rotación @p n y clave XOR @p k.
 *
 * Esta función recorre todas las combinaciones de valores de `n` (1 a 7) y `k` (0 a 255),
 * aplicando el descifrado con `decrypt_buffer()`. Luego, valida si el resultado
 * contiene un número suficiente de caracteres imprimibles usando `printable_ratio()`.
 *
 * Si el texto descifrado parece válido, intenta descomprimirlo primero con **RLE**
 * y después con **LZ78**, verificando si la salida contiene el fragmento conocido (`know_fragment`).
 * Si se encuentra coincidencia, retorna el mensaje descifrado, el método usado y los parámetros `n` y `k`.
 *
 * @param[in] enc            Puntero al buffer con el texto encriptado.
 * @param[in] enc_len        Longitud del buffer encriptado.
 * @param[in] know_fragment  Fragmento de texto conocido que debe aparecer en el mensaje descifrado.
 * @param[out] out_msg       Puntero de salida que contendrá el mensaje descifrado
 * @param[out] out_method    Puntero de salida que contendrá el método usado ("RLE" o "LZ78").
 * @param[out] out_n         Puntero de salida para el valor de rotación `n` encontrado.
 * @param[out] out_k         Puntero de salida para la clave XOR `k` encontrada.
 *
 * @return `true` si se encuentra una combinación válida que descifra y descomprime el mensaje,
 *         `false` en caso contrario.
 *
 */
bool finder(const uint8_t* enc, size_t enc_len, const char* know_fragment, char** out_msg, char** out_method, uint8_t* out_n, uint8_t* out_k);


#endif //SOLVER_HPP
