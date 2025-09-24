/**
 * @file solver.cpp
 * @brief implementación  de la función de busqueda de fuerza bruta
 *
 * Este modulo contiene:
 *      - finder: Metodo de busqueda por medio de fuerza bruta
 * Ademas cuenta con funciones auxiliares:
 *      - decrypt_buffer: Aplica las operaciones de bits para el desencriptado del texto
 *      - printable_ratio: Analiza el porcentaje del imprimible del texto para evitar analizar texto basura
 *      - contains_substr: Busca en el texto desencriptado si hay coincidencias con la pista
 *
 * @authors Julián Sánchez
 *          Jaider Bedoya
 *
 * @date 22/09/2025
 * @version 1.0
 */

#include <solver.hpp>

namespace
{

    /**
     * @brief Desencripta un buffer de datos aplicando una operación XOR y una rotación a la derecha.
     *
     * Esta función toma un buffer de entrada encriptado, aplica la operación XOR con la clave `k`
     * y luego rota cada byte `n` posiciones a la derecha, generando un nuevo buffer desencriptado.
     *
     * @param in   Puntero al buffer de entrada encriptado.
     * @param len  Longitud en bytes del buffer de entrada.
     * @param n    Número de bits a rotar hacia la derecha en cada byte.
     * @param k    Valor de la clave utilizada en la operación XOR.
     *
     * @return Puntero a un nuevo buffer dinámico con los datos desencriptados.
     *         El usuario es responsable de liberar la memoria con `delete[]`.
     *
     * @note La función utiliza `new[]` para reservar memoria.
     *       Asegúrese de liberar el buffer resultante para evitar fugas de memoria.
     */
    uint8_t *decrypt_buffer(const uint8_t *in, size_t len, uint8_t n, uint8_t k)
    {
        uint8_t *out = new uint8_t[len];

        for (size_t i = 0; i < len; i++)
        {
            uint8_t tmp = in[i] ^ k; // La inversa de la XOR es una XOR ajsdjas
            out[i] = ror_8(tmp, n);  // para invertir la rotación a la izquierda... se rota a la derecha!
        }

        return out;
    }

    /**
     * @brief Evalúa si un buffer contiene una proporción suficiente de caracteres ASCII imprimibles.
     *
     * Recorre el buffer y calcula la proporción de caracteres comprendidos en el rango ASCII imprimible (32–126).
     * Retorna verdadero si la proporción de caracteres imprimibles es mayor o igual al umbral `min_ratio`.
     *
     * @param buf        Puntero al buffer a analizar.
     * @param len        Longitud del buffer en bytes.
     * @param min_ratio  Proporción mínima aceptada (entre 0.0 y 1.0).
     *
     * @return `true` si el buffer cumple con la proporción mínima de caracteres imprimibles,
     *         `false` en caso contrario o si `len` es cero.
     *
     * @warning Si `len` es cero, la función imprime un mensaje de error y retorna `false`.
     */
    bool printable_ratio(const uint8_t *buf, size_t len, double min_ratio)
    {

        if (len == 0)
        {
            cout << "ERROR, No hay nada texto para analizar.\n";
            return false;
        }

        double cnt = 0;

        for (size_t i = 0; i < len; i++)
        {
            if (buf[i] >= 32 && buf[i] <= 126)
            { // cuantos de esos simbolos presentes en el texto son ASCII imprimibles?
                cnt++;
            }
        }

        double ratio = cnt / (double)len; // Razon de cuantos simbolos son imprimibles para evitar analizar texto basura

        return (ratio >= min_ratio);
    }

    /**
     * @brief Verifica si una cadena contiene una subcadena dada.
     *
     * Implementa una búsqueda secuencial de la subcadena `part` dentro del texto `text`.
     *
     * @param text  Cadena de texto principal donde buscar.
     * @param part  Subcadena que se desea localizar.
     *
     * @return `true` si `part` se encuentra dentro de `text`,
     *         `false` si no existe coincidencia o si los punteros son nulos.
     *
     * @warning Si `text` o `part` son punteros nulos, se imprime un mensaje de error y se retorna `false`.
     * @warning Si la longitud de `part` es mayor que la de `text`, la función imprime un mensaje de error y retorna `false`.
     *
     * @note La búsqueda distingue entre mayúsculas y minúsculas (case-sensitive).
     */
    bool contains_substr(const char *text, const char *part)
    {
        if (!text || !part)
        {
            cout << "Error, punteros nulos\n";
            return false;
        }

        size_t text_len = strlen(text);
        size_t part_len = strlen(part);

        if (part_len > text_len)
        {
            cout << "Error. Pista más grande que el texto original.\n";
            return false;
        }

        for (size_t i = 0; i + part_len <= text_len; i++)
        {
            size_t k = 0;

            // Comparar carácter por carácter
            while (k < part_len && text[i + k] == part[k])
            {
                k++;
            }

            // Si recorrimos todo el patrón, encontramos coincidencia
            if (k == part_len)
            {
                return true;
            }
        }

        return false;
    }

}

bool finder(const uint8_t *enc, size_t enc_len, const char *know_fragment, char **out_msg, char **out_method, uint8_t *out_n, uint8_t *out_k)
{

    *out_msg = nullptr;

    // ciclos de busqueda
    for (uint8_t n = 1; n <= 7; n++)
    { // busqueda del n
        for (unsigned int k = 0; k < 256; k++)
        { // busqueda del k

            // desencriptar cada posible combinación de n y k
            uint8_t *dec = decrypt_buffer(enc, enc_len, n, (uint8_t)k);

            if (!dec)
            {
                continue; // si retorna un nullptr pasa al siguiente k
            }
            if (!printable_ratio(dec, enc_len, RATIO_MIN))
            {
                delete[] dec; // ese dec no sirvió
                continue;     // pasamos al siguiente valor de k
            }

            // Si todo sale bien... desencriptemos!
            // RLE
            char *rle = rle_decompress(dec, enc_len);

            if (rle)
            {
                if (contains_substr(rle, know_fragment))
                {
                    *out_msg = rle;
                    *out_method = new char[4];
                    strcpy(*out_method, "RLE");
                    *out_n = n;
                    *out_k = (uint8_t)k;

                    delete[] dec;
                    return true;
                }
                delete[] rle;
            }

            // si llegamos aqui es porque no era RLE
            // LZ78
            char *lz = lz78_decompress(dec, enc_len);
            if (lz)
            {
                if (contains_substr(lz, know_fragment))
                {
                    *out_msg = lz;
                    *out_method = new char[6];
                    strcpy(*out_method, "LZ78");
                    *out_n = n;
                    *out_k = (uint8_t)k;

                    delete[] dec;
                    return true;
                }

                delete[] lz;
            }
            delete[] dec;
        }
    }

    return false;
}
