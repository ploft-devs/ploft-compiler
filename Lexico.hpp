#pragma once

#include "Token.hpp"
#include "Env.hpp"
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

class Lexico {
private:
std::queue<Token> tokens;
bool isLetter(char c);
bool isDigit(char c);
bool isTerminal(std::string s);
void reseve(Word w);
char ch=' ';
Env* env;
std::fstream* src;
public:
int line=1;
static std::vector<std::string> terminals;
static std::vector<char> letters;
static std::vector<char> digits;
Lexico (std::fstream* src, Env* env);
Token* getToken();
Lexico(){}

};
