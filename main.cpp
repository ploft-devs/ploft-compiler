#include "Lexico.hpp"
#include <fstream>


void lexerTest(){
    std::fstream* op = new  std::fstream("program6.plft");
    Env* amb = new Env();
    Lexico l(op,amb);
    Token* t;
    do{
        t = l.getToken();
        if(t->getTag()==Tag::ID) std::cout<<"\nID: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::NUM) std::cout<<"\nNUM: "<<((Num*)t)->value;
        else if(t->getTag()==Tag::EoF) std::cout<<"\nEOF";
        else std::cout<<"\nTAG: "<<t->getTag()<<"\tLEX: "<<((Word*)t)->lexeme;
        
    }while(t->getTag()!=Tag::EoF);
    std::cout<<"\nMap:\n";
    auto B = amb->table;
    for(auto it=B->begin();it!=B->end();it++) {
        std::cout<<(it->first)<<std::endl; 
    }
}

int main(){
    
    lexerTest();
    return 0;
}

