#include "Lexico.hpp"
Lexico::terminals = {"program", "declare", "begin", "end", "integer", "decimal", "if", "then", "else",
                     "do", "while", "for", "to", "read", "write", "not", "or", "mod", "and", "=", "+",
                     "-", "*", "/", "<", ">", "<=", ">=", "<>", "<<", ">>", ":=", ";", ",", (char)(34),
                     "?", ":"};

Lexico::letters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q",
                   "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h",
                   "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y"
                   "z"};
Lexico::digits= {"1", "2", "3", "4", "5", "6", "7", "8", "9"};          
Lexico::Lexico(std::fstream& src, Env* env){
    char buffer[5000];
    src.getline(buffer,5000);
    std::string ax = buffer;
    std::istringstream iss (ax);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},  std::istream_iterator<std::string>());
    while(src.good()){
        char buffer[5000];
        src.getline(buffer,5000);
        std::string line = buffer;

    }

}