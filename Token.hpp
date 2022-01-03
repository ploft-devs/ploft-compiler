#pragma once
#include<string>
#include "Tag.hpp"
class Token{
private:
    int tag;
public:
    Token();
    Token(int newTag):tag(newTag){};
    int getTag();
    Token* setTag(int newTag);
    std::string toString();
};

class Num:public Token{
    public:
    int value;

    Num(int val);

    std::string toString();
};

class Word:public Token{
    public:
    std::string lexeme;
    static Word And;
    static Word Or;
    static Word Eq;
    static Word Lt;
    static Word Gt;
    static Word Le;
    static Word Ge;
    static Word Ne;
    static Word True;
    static Word False;
    static Word Prog;
    static Word Beg;
    static Word End;
    static Word Int;
    static Word Dec;
    static Word Decl;
    static Word If;
    static Word Else;
    static Word Do;
    static Word While;
    static Word Read;
    static Word Write;
    static Word Ll;
    static Word Lll;
    static Word Gg;
    static Word Ggg;
    static Word Mod;
    static Word Then;
    static Word Add;
    static Word Sub;
    static Word Div;
    static Word Mlt;
    static Word Atrib;
    static Word Opar;
    static Word Cpar;
    static Word Smc;
    static Word Dqut;
    static Word Qst;
    static Word Clm;
    static Word Pnt;
    static Word Com;
    static Word For;

    Word(std::string lex, int tag);
    Word();
    

    std::string toString();
};