#pragma once
#include<string>
#include<fstream>
#include<vector>
#include<utility>
class Token{
private:
    std::string name;
    std::string value;
public:
    Token();
    Token(std::string newName, std::string newValue):name(newName),value(newValue);
    std::string getName();
    std::string getValue();
    std::pair<std::string,std::string> getPair();
    Token* setName(std::string newName);
    Token* setValue(std::string newValue);
    Token* setPair(std::pair<std::string,std::string>);
}

#include "Token.cpp"