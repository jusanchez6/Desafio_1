#include <compress.hpp>

namespace aux
{
    // Funciones auxiliares para la descompresión

    /**
     * @brief   Redimensiona un buffer dinámico, copiando su contenido al nuevo.
     *
     * @param   oldbuf   Puntero al buffer original (puede ser nullptr).
     * @param   oldcap   Capacidad en bytes del buffer original.
     * @param   newcap   Nueva capacidad deseada en bytes.
     *
     * @return  Un puntero al nuevo buffer de tamaño `newcap`.
     *          Si `oldbuf` no era nullptr, su contenido se copia
     *          y luego se libera la memoria del buffer anterior.
     */
    char *grow_and_copy(char *oldbuf, size_t oldcap, size_t newcap)
    {
        char *p = new char[newcap];     // Reserva un nuevo buffer con mayor capacidad
        if (oldbuf)                     // Si existía un buffer anterior...
        {
            memcpy(p, oldbuf, oldcap); // Copia el contenido del buffer viejo al nuevo
            delete[] oldbuf;           // Libera la memoria del buffer anterior
        }
        return p; // Retorna el nuevo buffer
    }

}

uint8_t ror_8(uint8_t v, unsigned int n)
{
    return (uint8_t)((v >> n) | (v << (8 - n)));
}

uint8_t rol_8(uint8_t v, unsigned int n)
{
    return (uint8_t)((v << n) | (v >> (8 - n)));
}

char *rle_decompress(const uint8_t *in, size_t len)
{
    size_t cap = 256;
    char *out = new char[cap]; // reserva memoria inicial para 256 bytes a la salida
    size_t out_len = 0;
    size_t i = 0;

    // lectura de digitos y su simbolo correspondiente
    while (i < len)
    {
        if (!isdigit(in[i]))
        {
            delete[] out;
            return nullptr;
        } // libera y retorna error si el primer char no es digito
        unsigned long count = 0;

        // contador de cantidad de veces que aparece el simbolo
        while (i < len && isdigit(in[i]))
        {
            count = count * 10 + (unsigned long)(in[i] - '0');
            if (count > 100000000UL)
            {
                delete[] out;
                return nullptr;
            } // libera y retorna error si es demasiado grande
            ++i;
        }
        if (i >= len)
        {
            delete[] out;
            return nullptr;
        } // libera y retorna error si la entrada termina luego del digito
        char sym = (char)in[i++]; // toma el simbolo que se repite count veces

        // redimensiona el arreglo si es necesario
        if (out_len + count + 1 > cap)
        {
            size_t newcap = cap;
            while (out_len + count + 1 > newcap)
                newcap *= 2;
            out = grow_and_copy(out, cap, newcap);
            cap = newcap;
        }

        // agrega cada sym count veces
        for (unsigned long k = 0; k < count; ++k)
            out[out_len++] = sym;
    }
    out[out_len] = '\0';
    return out;
}

char *lz78_decompress(const uint8_t *in, size_t len)
{

    size_t dict_cap = 1024;
    char **dict = new char *[dict_cap]; // diccionario de cadenas
    size_t dict_size = 0;

    size_t out_cap = 256;
    char *out = new char[out_cap]; // salida
    size_t out_len = 0;

    size_t i = 0;

    while (i < len)
    {

        if (!isdigit(in[i]))
        {
            // libera memoria y retorna error si el primer char no es digito
            for (size_t j = 0; j < dict_size; ++j)
            {
                delete[] dict[j];
            }
            delete[] dict;
            delete[] out;
            return nullptr;
        }

        unsigned long index = 0;

        while (i < len && isdigit(in[i]))
        {
            index = index * 10 + (unsigned long)(in[i] - '0');
            i++;
        }
        if (i >= len)
        {
            // libera memoria y retorna error si la entrada termina luego del digito
            for (size_t j = 0; j < dict_size; ++j)
            {
                delete[] dict[j];
            }
            delete[] dict;
            delete[] out;
            return nullptr;
        }

        char c = (char)in[i++]; // toma el simbolo

        char *s = nullptr;

        if (index == 0)
        {
            // caso especial: nueva cadena es solo el caracter c
            s = new char[2];
            s[0] = c;
            s[1] = '\0';
        }
        else
        {
            if (index > dict_size)
            {
                // libera memoria y retorna error si el indice es invalido
                for (size_t j = 0; j < dict_size; ++j)
                {
                    delete[] dict[j];
                }
                delete[] dict;
                delete[] out;
                return nullptr;
            }

            // nueva cadena es la cadena en dict[index - 1] + c
            char *prev = dict[index - 1];
            size_t prev_len = strlen(prev);
            s = new char[prev_len + 2];
            memcpy(s, prev, prev_len);
            s[prev_len] = c;
            s[prev_len + 1] = '\0';
        }

        // agrega s a la salida
        size_t s_len = strlen(s);
        if (out_len + s_len + 1 > out_cap)
        {

            size_t new_out_cap = out_cap;

            while (out_len + s_len + 1 > new_out_cap)
                new_out_cap *= 2;

            out = grow_and_copy(out, out_cap, new_out_cap);
            out_cap = new_out_cap;
        }

        memcpy(out + out_len, s, s_len);
        out_len += s_len;
        out[out_len] = '\0';

        // agrega s al diccionario
        if (dict_size + 1 > dict_cap)
        {
            size_t new_dict_cap = dict_cap * 2;
            char **new_dict = new char *[new_dict_cap];
            for (size_t j = 0; j < dict_size; ++j)
            {
                new_dict[j] = dict[j];
            }

            delete[] dict;
            dict = new_dict;
            dict_cap = new_dict_cap;
        }

        dict[dict_size++] = s;
    }
}
