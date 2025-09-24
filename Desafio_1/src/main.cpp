/**
 * @file main.cpp
 * @brief Punto de entrada principal del programa.
 *
 * Este archivo contiene la función `main`, la cual invoca a `app_main`
 * definida en @ref app.cpp para ejecutar la lógica principal de la aplicación.
 * 
 * @authors Julián Sánchez
 *          Jaider Bedoya
 * 
 * @date 22/09/2025
 * @version 1.00
 */

#include <QCoreApplication>
#include <app.hpp>

/**
 * @brief Función principal de la aplicación.
 *
 * Inicia el programa y delega la ejecución a `app_main`.
 *
 * @return 0 si la aplicación terminó correctamente.
 * @return 1 si ocurrió algún error.
 */
int main()
{
    // app_main devuelve my_error_t:
    // - OK (1) → éxito
    // - ERROR (0) → fallo
    //
    // Se convierte a valor de retorno estándar:
    //  - 0 → éxito (convención UNIX)
    //  - 1 → error
    return app_main() ? 0 : 1;
}
