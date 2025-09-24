/**
 * @file app.cpp
 * @brief Implementación de las funciones principales de la aplicación.
 *
 * Este archivo contiene la lógica principal (`app_main`) y funciones auxiliares
 * para leer archivos y extraer fragmentos de texto.
 * 
 * @authors Julián Sánchez
 *          Jaider Bedoya
 * 
 * @date 22/09/2025
 * @version 1.00
 */

#include <app.hpp>

namespace
{
    /**
     * @brief Obtiene el tamaño de un archivo en bytes.
     *
     * Abre el archivo en modo binario, mueve el puntero al final,
     * consulta su posición (que corresponde al tamaño), y vuelve
     * al inicio antes de cerrar.
     *
     * @param[in] path Ruta al archivo.
     * @return Tamaño del archivo en bytes o NO_SIZE si ocurre un error.
     */
    size_t get_file_size(const char *path)
    {
        // Abrir fichero en modo binario
        FILE *f = fopen(path, "rb");

        if (!f)
        {
            fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", path);
            return NO_SIZE;
        }

        // Posicionar puntero al final del archivo
        if (fseek(f, 0, SEEK_END) != 0)
        {
            fprintf(stderr, "Error: fallo al mover el puntero al final del archivo\n");
            fclose(f);
            return NO_SIZE;
        }

        // ftell devuelve la posición actual del puntero
        long sz = ftell(f);

        if (sz < 0)
        {
            fprintf(stderr, "Error: fallo al obtener el tamaño del archivo\n");
            fclose(f);
            return NO_SIZE;
        }

        // Reposicionar puntero al inicio para próximas lecturas
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fprintf(stderr, "Error: fallo al reposicionar el puntero al inicio\n");
            fclose(f);
            return NO_SIZE;
        }

        fclose(f);
        return static_cast<size_t>(sz); // Conversión explícita a size_t
    }

}; // namespace


my_error_t app_main()
{
    int n;
    cout << "Ingrese el numero de archivos para la prueba: ";
    cin >> n;

    if (n < 1)
    {
        cout << "Argumento invalido." << endl;
        return ERROR;
    }

    // Procesar n archivos con sus respectivas pistas
    for (int i = 1; i <= n; i++)
    {
        char enc_name[64];
        char pista_name[64];

        // Genera nombres de archivo dinámicamente: EncriptadoX.txt y pistaX.txt
        snprintf(enc_name, sizeof(enc_name), "Encriptado%d.txt", i);
        snprintf(pista_name, sizeof(pista_name), "pista%d.txt", i);

        // ====== Lectura del archivo encriptado ============
        uint8_t *enc = nullptr;
        size_t enc_len = 0;

        if (!read_file(enc_name, &enc, &enc_len))
        {
            cout << "No se pudo leer: " << enc_name << endl;
            continue;
        }

        // ====== Lectura de la pista ============
        char* frag = get_frag(pista_name);

        if (!frag)
        {
            cout << "No se pudo leer: " << pista_name << endl;
            delete[] enc; // Liberar memoria reservada
            continue;
        }

        cout << "** " << enc_name << " **\n";

        // Variables de salida del solver
        char *out_msg = nullptr;
        char *out_method = nullptr;
        uint8_t out_n = 0;
        uint8_t out_k = 0;

        // Buscar coincidencia con la pista usando el solver
        bool result = finder(enc, enc_len, frag, &out_msg, &out_method, &out_n, &out_k);

        if (result) {
            // Imprimir resultados de la búsqueda
            cout << "Compresión: " << out_method << endl;
            cout << "Rotación: " << (int)out_n << "\n";
            cout << "k: " << (int)out_k << endl;
            cout << "Mensaje: \n\n" << out_msg << endl;

            // Liberar memoria reservada por solver
            delete[] out_msg;
            delete[] out_method;

        } else {
            cout << "No se encontró coincidencia con la pista.\n";
        }

        // Liberar memoria usada en este ciclo
        delete[] enc;
        delete[] frag;
    }

    return OK;
}


my_error_t read_file(const char *path, uint8_t **out_buf, size_t *out_len)
{
    size_t sz = get_file_size(path);
    if (!sz) {
        return ERROR;
    }

    FILE *f = fopen(path, "r");
    if (!f) {
        cout << "Error: No se pudo abrir el archivo: " << path << "\n";
        return ERROR;
    }

    // Reservar buffer y leer contenido
    uint8_t *buf = new uint8_t[sz];
    size_t r = fread(buf, 1, sz, f);
    fclose(f);

    if (r != sz) {
        cout << "Error: No se leyó el archivo completo.\n";
        delete[] buf;
        return ERROR;
    }

    *out_buf = buf;
    *out_len = sz;
    return OK;
}


char *get_frag(const char *path)
{
    uint8_t* buf = nullptr;
    size_t len = 0;

    if (read_file(path, &buf, &len) != OK) {
        return nullptr;
    }

    // Crear copia en formato C-string
    char *frag = new char[len + 1];
    memcpy(frag, buf, len);
    frag[len] = '\0';

    delete[] buf; // liberar el buffer original
    return frag;
}
