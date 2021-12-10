#include<string>
#include<fstream>
#include<vector>

std::string Token::getName(){
    return this->name;
}

std::string Token::getValue(){
    return this->value;
}

std::pair<std::string,std::string> Token::getPair(){
    return std::pair<std::string,std::string>(this->name, this->value);
}

Token* setName(std::string newName){
    this->name=newName;
    return this;
}

Token* setValue(std::string newValue){
    this->value=newValue;
    return this;
}

Token* setPair(std::pair<std::string,std::string> pair){
    this->name=pair.first();
    this->value=pair.second();
    return this;
}