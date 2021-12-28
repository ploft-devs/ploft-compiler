#include "Env.hpp"

Env::Env(Env* n){
    prev = n;
}

void Env::put(Token tk, Id id){
    table->[tk] = id;
}

Id Env::get(Token tk){
    auto search = this->table->find(tk);
    if (search != this->table->end())
        return search->second;
    return Id();
}