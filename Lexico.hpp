#pragma once

#include "Token.hpp"
#include "Env.hpp"
#include <queue>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>

class Lexico {
private:
std::queue<Token> tokens;
Env* env;
public:
static std::vector<std::string> terminals;
static std::vector<std::string> letters;
static std::vector<std::string> digits;
Lexico (std::fstream& src, Env* env);
Token getToken();

};
