#ifndef APP_H
#define APP_H

#include <iostream>
#include <cstdio>

#include <compress.hpp>
#include <solver.hpp>


#define NO_SIZE 0

/**
 * @brief Declaración del tipo my_error_t para el manejo de excepciones y errores
 */
typedef enum {
    ERROR,      // ERROR GENERAL
    OK,         // OK
    TIMEOUT     // TIMEOUT, NO CREO USARLO
} my_error_t;


using namespace std;

/**
 * @brief Contiene la logica y el flujo de la aplicación
 *
 * @return OK: si todo salió bien
 */
my_error_t app_main();

/**
 * @brief Lee un archivo .txt
 *
 * @param path: Ubicación del archivo
 * @param out_buf: Puntero donde se guardara lo que se lea del archivo
 * @param out_len: tamaño de la lectura
 *
 * @return OK: si todo salió bien.
 */
my_error_t read_file (const char* path, uint8_t** out_buf, size_t* out_len);

char* get_frag(const char* path);

#endif  //APP_HPP
