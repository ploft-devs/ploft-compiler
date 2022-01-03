#include "Token.hpp"

int Token::getTag(){
    return this->tag;
}
Token::Token(){
}
std::string Token::toString(){
    return std::to_string(tag);
}

Num::Num(int val):Token(Tag::NUM){
    value=val;
}

std::string Num::toString(){
    return std::to_string(value);
}

Word::Word(std::string lex, int tag):Token(tag){
    lexeme=lex;
}

Word::Word():Token(){
    lexeme="NULL";
}

std::string Word::toString(){
    return lexeme;
}

//Palavras reservadas

//ops
Word Word::And = Word("and",Tag::AND);
Word Word::Or = Word("or",Tag::OR);
Word Word::Eq = Word("=",Tag::EQ);
Word Word::Ne = Word("<>",Tag::NE);
Word Word::Lt = Word("<",Tag::LT);
Word Word::Gt = Word(">",Tag::GT);
Word Word::Le = Word("<=",Tag::LE);
Word Word::Ll = Word("<<",Tag::LL);
Word Word::Lll = Word("<<<",Tag::LLL);
Word Word::Ge = Word(">=",Tag::GE);
Word Word::Gg = Word(">>",Tag::GG);
Word Word::Ggg = Word(">>>",Tag::GGG);
Word Word::Mod = Word("mod",Tag::MOD);
Word Word::Add = Word("+",Tag::ADD);
Word Word::Sub = Word("-",Tag::SUB);
Word Word::Mlt = Word("*",Tag::MLT);
Word Word::Div = Word("/",Tag::DIV);
Word Word::Atrib = Word(":=",Tag::ATRIB);
Word Word::Opar = Word("(",Tag::OPAR);
Word Word::Cpar = Word(")",Tag::CPAR);
Word Word::Smc = Word(";",Tag::SMC);
Word Word::Dqut = Word("\"",Tag::DQUT);
Word Word::Qst = Word("?",Tag::QST);
Word Word::Clm = Word(":",Tag::CLM);
Word Word::Pnt = Word(".",Tag::PNT);
Word Word::Com = Word(",",Tag::COM);





//Key Words
Word Word::Prog = Word("program",Tag::PRG);
Word Word::Beg = Word("begin",Tag::BGN);
Word Word::End = Word("end",Tag::END);
Word Word::Decl = Word("declare",Tag::DECL);
Word Word::True = Word("true",Tag::TRUE);
Word Word::False = Word("false",Tag::FALSE);
Word Word::If = Word("if",Tag::IF);
Word Word::Else = Word("else",Tag::ELSE);
Word Word::Then = Word("then",Tag::THEN);
Word Word::Do = Word("do",Tag::DO);
Word Word::While = Word("while",Tag::WHILE);
Word Word::For = Word("for",Tag::FOR);
Word Word::Int = Word("integer",Tag::INT);
Word Word::Dec = Word("decimal",Tag::DEC);
Word Word::Read = Word("read",Tag::READ);
Word Word::Write = Word("write",Tag::WRT);