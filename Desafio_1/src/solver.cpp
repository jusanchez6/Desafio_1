#include <solver.hpp>

uint8_t ror_8 (uint8_t v, unsigned int n) {
    return (uint8_t)((v >> n) | (v << (8-n)));
}

uint8_t rol_8 (uint8_t v, unsigned int n) {
    return (uint8_t)((v << n)| (v >> (8-n)));
}
