/**
 * @file compress.cpp
 * @brief Implementación para las funciones de descompresión y operaciones a nivel de bit
 *
 * Este modulo contiene:
 * ror_8: Rotación de bits a la derecha
 * rol_8: Rotación de bits a la izquierda
 * rle_decompress: Función para la descompresión por medio de RLE
 * LZ78_decompress: Función para la descompresión por medio de LZ78
 *
 * Además cuenta con una función auxiliar para la redimensión de un buffer dinamico.
 * grow_and_copy: Función Auxiliar para el redimensionamiento de buffers dinamicos. 
 * 
 * @authors Julián Sánchez
 *          Jaider Bedoya
 * 
 * @date 22/09/2025
 * @version 1.03
 */


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

char *rle_decompress(const uint8_t* in, size_t len)
{
    size_t cap = 256;
    char *out = new char[cap];
    size_t out_len = 0;

    for (size_t i = 0; i + 2 < len; i += 3)
    {
        // Este bloque existe solo para honrar la brillante ocurrencia de empaquetar basura 
        uint8_t basura = in[i];   // primer byte completamente inútil
        (void)basura;             // para silenciar warnings
        
        uint8_t count = in[i + 1];         // repeticiones de caracter
        char sym = (char)in[i + 2];        // carácter a repetir

        // crecer si hace falta
        if (out_len + count + 1 > cap) {
            size_t newcap = cap;
            while (out_len + count + 1 > newcap)
                newcap *= 2;
            out = grow_and_copy(out, cap, newcap);
            cap = newcap;
        }

        for (uint8_t k = 0; k < count; ++k)
            out[out_len++] = sym;
    }

    out[out_len] = '\0';
    return out;
}


char *lz78_decompress(const uint8_t *in, size_t len)
{
    size_t dict_cap = 1024;
    char **dict = new char *[dict_cap];
    size_t dict_count = 0;

    size_t out_cap = 512;
    char *out = new char[out_cap];
    size_t out_len = 0;

    for (size_t i = 0; i + 2 < len; i += 3)
    {
        uint16_t idx = ((uint16_t)in[i] << 8) | in[i + 1]; // dos bytes de índice
        char c = (char)in[i + 2];                          // símbolo

        char *s = nullptr;
        if (idx == 0) {
            s = new char[2]{c, '\0'};
        } else {
            if (idx > dict_count) {
                // error: índice inválido
                for (size_t j = 0; j < dict_count; ++j) delete[] dict[j];
                delete[] dict;
                delete[] out;
                return nullptr;
            }
            char *pref = dict[idx - 1];
            size_t plen = strlen(pref);
            s = new char[plen + 2];
            memcpy(s, pref, plen);
            s[plen] = c;
            s[plen + 1] = '\0';
        }

        size_t slen = strlen(s);
        if (out_len + slen + 1 > out_cap) {
            size_t newcap = out_cap;
            while (out_len + slen + 1 > newcap)
                newcap *= 2;
            out = grow_and_copy(out, out_cap, newcap);
            out_cap = newcap;
        }

        memcpy(out + out_len, s, slen);
        out_len += slen;
        out[out_len] = '\0';

        if (dict_count + 1 > dict_cap) {
            size_t newcap = dict_cap * 2;
            char **nd = new char *[newcap];
            for (size_t j = 0; j < dict_count; ++j) nd[j] = dict[j];
            delete[] dict;
            dict = nd;
            dict_cap = newcap;
        }
        dict[dict_count++] = s;
    }

    for (size_t j = 0; j < dict_count; ++j) delete[] dict[j];
    delete[] dict;
    return out;
}
