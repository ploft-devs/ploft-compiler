#include "Lexico.hpp"
#include "Syntax.hpp"
#include <fstream>
#include <cstdlib>


void lexerTest(std::string ss){
    std::fstream* op = new  std::fstream(ss);
    std::ofstream tab ("lex-"+ss+".txt");
    Env* amb = new Env();
    Lexico l(op,amb);
    Token* t;
    do{
        t = l.getToken();
        if(t->getTag()==Tag::ID) tab<<"\nID: "<<((Word*)t)->lexeme;
        else if(t->getTag()==Tag::NUM) tab<<"\nNUM: "<<((Num*)t)->value;
        else if(t->getTag()==Tag::EoF) tab<<"\nEOF";
        else tab<<"\nTAG: "<<t->getTag()<<"\tLEX: "<<((Word*)t)->lexeme;
        
    }while(t->getTag()!=Tag::EoF);
    std::cout<<"\nMap:\n";
    auto B = amb->table;
    for(auto it=B->begin();it!=B->end();it++) {
        if(it->second.getTag()==Tag::ID)
            std::cout<<(it->first)<<"\tType: "<<(it->second).type<<std::endl;
        else
            std::cout<<(it->first)<<"\tTag: "<<it->second.getTag()<<std::endl; 
    }
}

void syntaxTest(std::string ss, std::string out){
    std::fstream* op = new  std::fstream(ss);
    Env* amb = new Env();
    std::ofstream tab ("tabSym-"+ss+".txt");
    Lexico l(op,amb);
    Syntax s(l, out);
    s.startAnalysis();
    std::cout<<"\nMap:\n";
    auto B = amb->table;
    for(auto it=B->begin();it!=B->end();it++) {
        if(it->second.getTag()==Tag::ID)
            tab<<"ID: "<<(it->first)<<"\tType: "<<(it->second).type<<std::endl;
        else 
            tab<<(it->first)<<"\tTag: "<<it->second.getTag()<<std::endl; 
    }

}
void compile(std::string ss){
    std::string com = "g++ "+ss+".cpp -o "+ss+".exe";
    std::system(com.c_str());
}
int main(int argc, char** argv){
    if(argc!=3){
        std::cout<<"Numero de argumentos errado!!\n O primeiro argumento deve ser o nome do arquivo a ser compilado e o segundo o nome do arquivo a ser gerado";
        return 0;
    }
    std::cout<<"\nCompilando o arquivo: "<<std::string{argv[1]};

    lexerTest(std::string{argv[1]});
    syntaxTest(std::string{argv[1]}, std::string{argv[2]});
    compile(std::string{argv[2]});
    return 0;
}



