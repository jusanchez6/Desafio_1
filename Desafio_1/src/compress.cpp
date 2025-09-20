#include <compress.hpp>

uint8_t ror_8 (uint8_t v, unsigned int n) {
    return (uint8_t)((v >> n) | (v << (8-n)));
}

uint8_t rol_8 (uint8_t v, unsigned int n) {
    return (uint8_t)((v << n)| (v >> (8-n)));
}

char* rle_decompress(const uint8_t* in, size_t len) {
    size_t cap = 256;
    char* out = new char[cap]; // reserva memoria inicial para 256 bytes a la salida
    size_t out_len = 0;
    size_t i = 0;

    // lectura de digitos y su simbolo correspondiente
    while (i < len) {
        if (!isdigit(in[i])) { delete[] out; return nullptr; } // libera y retorna error si el primer char no es digito
        unsigned long count = 0;

        //contador de cantidad de veces que aparece el simbolo
        while (i < len && isdigit(in[i])) {
            count = count * 10 + (unsigned long)(in[i] - '0');
            if (count > 100000000UL) { delete[] out; return nullptr; }// libera y retorna error si es demasiado grande
            ++i;
        }
        if (i >= len) { delete[] out; return nullptr; } //libera y retorna error si la entrada termina luego del digito
        char sym = (char)in[i++]; //toma el simbolo que se repite count veces

        // redimensiona el arreglo si es necesario
        if (out_len + count + 1 > cap) {
            size_t newcap = cap;
            while (out_len + count + 1 > newcap) newcap *= 2;
            out = grow_and_copy(out, cap, newcap);
            cap = newcap;
        }

        // agrega cada sym count veces
        for (unsigned long k = 0; k < count; ++k) out[out_len++] = sym;
    }
    out[out_len] = '\0';
    return out;
}
