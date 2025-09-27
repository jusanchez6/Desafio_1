/**
 * @file app.hpp
 * @brief Archivo de cabecera para la declaración de las funciones principales de la aplicación.
 *
 * Este archivo contiene las definiciones y prototipos de funciones
 * necesarias para el manejo de archivos, ejecución principal y
 * control de errores de la aplicación.
 * 
 * @authors Julián Sánchez,
 *          Jaider Bedoya
 * 
 * @date 22/09/2025
 * @version 1.00
 */

#ifndef APP_H
#define APP_H

#include <iostream>
#include <cstdio>

#include <compress.hpp>
#include <solver.hpp>

#define NO_SIZE 0

/**
 * @enum my_error_t
 * @brief Enumeración para el manejo de errores y excepciones.
 *
 * Define los posibles códigos de retorno que pueden generarse
 * durante la ejecución de la aplicación.
 */
typedef enum {
    ERROR,      /**< Error general en la ejecución. */
    OK,         /**< Ejecución correcta. */
    TIMEOUT     /**< Operación expirada (no se utiliza actualmente). */
} my_error_t;

using namespace std;

/**
 * @brief Punto de entrada principal de la aplicación.
 *
 * Contiene la lógica y el flujo de la aplicación, ejecutando
 * los módulos principales.
 *
 * @return OK si la ejecución fue exitosa.
 * @return ERROR si ocurrió un error durante la ejecución.
 */
my_error_t app_main();

/**
 * @brief Lee el contenido de un archivo de texto.
 *
 * Abre el archivo indicado en @p path y guarda su contenido en
 * un buffer dinámico. La memoria debe ser liberada por el
 * llamador.
 *
 * @param[in] path Ruta al archivo de entrada.
 * @param[out] out_buf Doble puntero donde se almacenará el buffer leído.
 * @param[out] out_len Tamaño total en bytes del contenido leído.
 *
 * @return OK si el archivo se leyó correctamente.
 * @return ERROR si ocurrió un fallo al abrir o leer el archivo.
 */
my_error_t read_file(const char* path, uint8_t** out_buf, size_t* out_len);

/**
 * @brief Obtiene un fragmento de información de un archivo.
 *
 * Función auxiliar para extraer información específica de un archivo
 * de texto.
 *
 * @param[in] path Ruta al archivo de entrada.
 *
 * @return Puntero a una cadena de caracteres con el fragmento extraído.
 *         El llamador es responsable de liberar la memoria reservada.
 */
char* get_frag(const char* path);

#endif // APP_H
