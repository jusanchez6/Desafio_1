#include <solver.hpp>

namespace
{

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

    bool contains_substr(const char *text, const char *part)
    {
        if (!text || !part)
        {
            cout << "Error, punteros nulos\n";
            return false;
        }

        size_t text_len = strlen(text);
        size_t part_len = strlen(part);

        // Caso especial: patrón vacío
        if (part_len == 0)
        {
            return true; // "" siempre está contenido
        }

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
        for (uint8_t k = 0; k <= 255; k++)
        { // busqueda del k

            // desencriptar cada posible combinación de n y k
            uint8_t *dec = decrypt_buffer(enc, enc_len, n, k);

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
                if (contains_substr(rle, know_fragment)) {
                    *out_msg = rle;
                    *out_method = new char[4];
                    strcpy(*out_method, "RLE");
                    *out_n = n;
                    *out_k = k;

                    delete[] dec;
                    return true;
                }
                delete[] rle;
            }

            // si llegamos aqui es porque no era RLE
            //LZ78
            char* lz = lz78_decompress(dec, enc_len);

            if (lz) {
                if (contains_substr(lz, know_fragment)) {
                    *out_msg = lz;
                    *out_method = new char[6];
                    strcpy(*out_method, "LZ78");
                    *out_n = n;
                    *out_k = k;

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