#pragma once

#include <unordered_map>
#include "Token.hpp"

class Env{

public:
std::unordered_map<std::string, Word>* table;
Env ();
void put(std::string tk, Word w);
Word* get(std::string tk);

};
