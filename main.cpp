#include "Lexico.hpp"
#include <fstream>
int main(){
    std::fstream* op = new  std::fstream("teste.txt");
    Env amb;
    Lexico l(op,&amb);
    while(op->good()){
        Token* t = l.getToken();
        if(t->getTag()==Tag::ID) std::cout<<"\nID: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::LE) std::cout<<"\nSYM: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::INT) std::cout<<"\nINT: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::SMC) std::cout<<"\nSMC: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::ATRIB) std::cout<<"\nSYM: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::PNT) std::cout<<"\nPNT: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::LLL) std::cout<<"\nLLL: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::NUM) std::cout<<"\nNUM: "<<((Num*)t)->value;
        else std::cout<<"\nToken Nao reconhecido com Tag: "<<t->getTag();
    }
   
    return 0;
}