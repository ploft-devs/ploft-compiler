#include "Lexico.hpp"
std::vector<std::string> Lexico::terminals = {"program", "declare", "begin", "end", "integer", "decimal", "if", "then", "else",
                     "do", "while", "for", "to", "read", "write", "not", "or", "mod", "and", "=", "+",
                     "-", "*", "/", "<", ">", "<=", ">=", "<>", "<<", ">>", "<<<", ">>>", ":=", ";", ",", "\"",
                     "?", ":", ".", "(", ")"};               

std::vector<char> Lexico::letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                   'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                   'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
                   'z'};
bool Lexico::isLetter(char c){
    return std::find(letters.begin(), letters.end(),c)!=letters.end();
}

std::vector<char> Lexico::digits= {'0','1', '2', '3', '4', '5', '6', '7', '8', '9'};   

bool Lexico::isDigit(char c){
    return std::find(digits.begin(), digits.end(),c)!=digits.end();
}
bool Lexico::isTerminal(std::string c){
    return std::find(terminals.begin(), terminals.end(),c)!=terminals.end();
}
Lexico::Lexico(std::fstream* src, Env* env){
    this->env = env;
    this->src = src;

    reseve(Word::If);
    reseve(Word::Else);
    reseve(Word::Prog);
    reseve(Word::Beg);
    reseve(Word::End);
    reseve(Word::While);
    reseve(Word::Do);
    reseve(Word::Then);
    reseve(Word::For);
    reseve(Word::And);
    reseve(Word::Or);
    reseve(Word::Mod);
    reseve(Word::Int);
    reseve(Word::Dec);
    reseve(Word::Decl);
    reseve(Word::True);
    reseve(Word::False);
    reseve(Word::Read);
    reseve(Word::Write);

    /*
    
        else if(c=='\"'){
            c = src->get();
            while(c!='\"'){
                
            }
        }
        else {
            std::cout<<"\n->Caractere invalido: "<<c;
            c = src->get();
        }
        
    }
    if(isTerminal(newToken)){
        std::cout<<"\n->Terminal: "<<newToken;
    }
    */
}
Token* Lexico::getToken(){
    if(!src->good()){
        Token* a = new Token(Tag::EoF);
        return a;
    }
    while(ch=='\t'||ch=='\r'||ch=='\b'||ch=='\n'||ch==' '|| ch=='%'){
        if(ch=='\n') line++;
        ch = src->get();
        if(ch=='%'){
            while(ch!='\n') ch = src->get();
            line++;
        }
        
    }
    
    while(ch=='\t'||ch=='\r'||ch=='\b'||ch=='\n'||ch==' '){
        if(ch=='\n') line++;
        ch = src->get();
    }
    switch(ch){
        case '"': {
            std::string lit="";
            do{
                lit+=ch;
                ch = src->get();
            }while(ch!='"');
            Word * n = new Word(lit,Tag::LIT);
            return n;
        }
        case '=': {
            ch = src->get();
            return &Word::Eq;
        }
        case ':': 
            ch = src->get();
            if(ch=='='){
                ch = src->get(); 
                return &Word::Atrib;
            }
            return &Word::Clm;
        case '<':
            ch = src->get();
            if(ch=='='){
                ch = src->get();
                return &Word::Le;
            }
            if(ch=='>'){
                ch = src->get();
                return &Word::Ne;
            }
            if(ch=='<'){
                ch=src->get();
                if(ch=='<'){
                    ch = src->get();
                    return &Word::Lll;
                }
                return &Word::Ll;
            }
            return &Word::Lt;
        case '>':
            ch=src->get();
            if(ch=='>'){
                ch = src->get();
                if(ch=='>') {
                    ch = src->get();
                    return &Word::Ggg;
                }
                return &Word::Gg;
            }
            if(ch=='=') {
                ch = src->get();
                return &Word::Ge;
            }
            return &Word::Gt;
        case '+': ch = src->get();
            return &Word::Add;
        case '-': ch = src->get();
            return &Word::Sub;
        case '*': ch = src->get();
            return &Word::Mlt;
        case '/': ch = src->get();
            return &Word::Div;
        case '(': ch = src->get();
            return &Word::Opar;
        case ')': ch = src->get();
            return &Word::Cpar;
        case ';': ch = src->get();
            return &Word::Smc;
        case '?': ch = src->get();
            return &Word::Qst;
        case '.': ch = src->get();
            return &Word::Pnt;
        case ',': ch = src->get();
            return &Word::Com;
    }
    if(isDigit(ch)){
        int val=0;
        do{
            val = 10*val + stoi(std::string{ch});
            ch = src->get();
        } while(isDigit(ch));
        Num* n = new Num(val);
        return n;
    }

    if(isLetter(ch)){
        std::string word="";
        do{
            word+=ch;
            ch = src->get();
        }while(isLetter(ch)||isDigit(ch));
        Word* x = env->get(word);
        if(x) return x;
        x = new Word(word, Tag::ID);
        env->put(word,*x);
        return x;
    }
    std::cout<<"\nLexer error: Unknown token '"<<ch<<"' on line "<<line<<".";
    Word* a = new Word(std::string{ch}, -1);
    ch = src->get();
    return a;
}
void Lexico::reseve(Word w){
    env->put(w.lexeme, w);
}