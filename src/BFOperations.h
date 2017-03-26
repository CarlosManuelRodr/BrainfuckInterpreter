#ifndef BFOPERATIONS_H
#define BFOPERATIONS_H

#include <string>

// Simple Brainfuck operations used to encode text.
std::string bf_sum(char n);
std::string bf_multiply(char a, char b);
std::string bf_write_char(char n);
std::string text_to_bf(std::string text);

#endif