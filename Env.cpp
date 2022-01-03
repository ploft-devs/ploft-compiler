#include "Env.hpp"

Env::Env(){
    table = new std::unordered_map<std::string, Word>;
}

void Env::put(std::string tk, Word id){
    table->insert({tk,id});
}

Word* Env::get(std::string tk){
    auto search = this->table->find(tk);
    if (search != this->table->end()){
        Word* w = new Word(search->second.lexeme, search->second.getTag());
        return w;
    }
    return nullptr;
}