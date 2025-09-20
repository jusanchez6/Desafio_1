#include <compress.hpp>

namespace
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
        char *p = new char[newcap]; // Reserva un nuevo buffer con mayor capacidad
        if (oldbuf)                 // Si existía un buffer anterior...
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
    size_t dict_cap = 1024;             // capacidad inicial del diccionario
    char **dict = new char *[dict_cap]; // arreglo de punteros a cadenas
    size_t dict_count = 0;              // cuántas entradas reales hay en el diccionario

    size_t out_cap = 512;          // capacidad inicial de la salida
    char *out = new char[out_cap]; // buffer dinámico para salida
    size_t out_len = 0;            // longitud de salida actual

    size_t i = 0;
    while (i < len)
    { // procesar mientras haya datos en la entrada
        if (!isdigit(in[i]))
        { // error: cada token debe empezar con un índice (dígito)
            for (size_t j = 0; j < dict_count; ++j)
                delete[] dict[j];
            delete[] dict;
            delete[] out;
            return nullptr;
        }

        unsigned long idx = 0; // índice leído del stream comprimido
        while (i < len && isdigit(in[i]))
        { // convertir los dígitos en número
            idx = idx * 10 + (unsigned long)(in[i] - '0');
            ++i;
        }

        if (i >= len)
        { // error: la entrada no puede terminar justo después del índice
            for (size_t j = 0; j < dict_count; ++j)
                delete[] dict[j];
            delete[] dict;
            delete[] out;
            return nullptr;
        }

        char c = (char)in[i++]; // caracter literal que acompaña al índice

        char *s = nullptr;
        if (idx == 0)
        { // caso especial: cadena nueva es solo 'c'
            s = new char[2];
            s[0] = c;
            s[1] = '\0';
        }
        else
        {
            if (idx > dict_count)
            { // error: índice fuera de rango
                for (size_t j = 0; j < dict_count; ++j)
                    delete[] dict[j];
                delete[] dict;
                delete[] out;
                return nullptr;
            }
            char *pref = dict[idx - 1]; // recuperar cadena previa del diccionario
            size_t plen = strlen(pref);
            s = new char[plen + 2]; // construir nueva cadena = prefijo + c
            memcpy(s, pref, plen);
            s[plen] = c;
            s[plen + 1] = '\0';
        }

        size_t slen = strlen(s); // longitud de la nueva cadena
        if (out_len + slen + 1 > out_cap)
        { // si no cabe en salida → crecer buffer
            size_t newcap = out_cap;
            while (out_len + slen + 1 > newcap)
                newcap *= 2;
            out = grow_and_copy(out, out_cap, newcap);
            out_cap = newcap;
        }
        memcpy(out + out_len, s, slen); // copiar s al buffer de salida
        out_len += slen;
        out[out_len] = '\0'; // mantener la salida como string C

        // agregar s al diccionario
        if (dict_count + 1 > dict_cap)
        { // si no cabe, duplicar la capacidad
            size_t newcap = dict_cap * 2;
            char **nd = new char *[newcap];
            for (size_t j = 0; j < dict_count; ++j)
                nd[j] = dict[j];
            delete[] dict;
            dict = nd;
            dict_cap = newcap;
        }
        dict[dict_count++] = s; // guardar puntero a la nueva cadena
    }

    // liberar memoria del diccionario (no se necesita más)
    for (size_t j = 0; j < dict_count; ++j)
        delete[] dict[j];
    delete[] dict;

    return out; // retornar la cadena descomprimida
}
