#pragma once

#include <unordered_map>
#include "Token.hpp"
#include "Id.hpp"

class Env{
private:
std::unordered_map<Token>* table;

public:

Env ();
void put(Token tk, Id id);
Id get(Token tk);

};
