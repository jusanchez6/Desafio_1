#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <iostream>
#include <cstdint>
#include <cctype>
#include <cstring>

#include <compress.hpp>

#define RATIO_MIN 0.6

using namespace std;


bool finder(const uint8_t* enc, size_t enc_len, const char* know_fragment, char** out_msg, char** out_method, uint8_t* out_n, uint8_t* out_k);


#endif //SOLVER_HPP
