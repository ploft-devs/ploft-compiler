#include "Lexico.hpp"
#include <stack>
class Syntax{
    private:
    std::stack<Token*> tokens;

    public:
    Token* nextToken(){
        if(tokens.empty()){
            Token * x = lex.getToken();
            std::cout<<"\nToken recebido do lexico: "<<x->getTag();
            return x;
        }
        else{
            auto x = tokens.top();
            tokens.pop();
            return x;
        }
    }

    void addToken(Token*t){
        std::cout<<"\nToken identificado: "<<t->getTag();
        tokens.push(t);
    }
    Lexico lex;
    Syntax(Lexico l){
        lex=l;
    }
    void erro(std::string s, int i){
        std::cout<<"\nSyntax error on line "<<lex.line<<": Exepect "<<s<<", but found token with ID="<<i<<" !!\n Ignoring tokens...";
    }

    //S->*program id body $
    bool startAnalysis(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::PRG){
                b= b&&e1();
            }
            else
                erro("key-word 'program'", t->getTag());
            t = nextToken();
        }
        if(b==false)
            std::cout<<"\nFailed parsing!!";
        else
            std::cout<<"\nParsing successful!!";
        return b;
    }

    //S->program *id body $
    bool e1(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::ID){
                b= b&&e2();
            }
            else{
                erro("identifier", t->getTag());
                b=false;
            }
            
            t = nextToken();
        }
        return b;
    }

    //S->program id *body $
    //body->*declare decl-lst begin stmt-lst end
    //body->*begin stmt-lst end
    bool e2(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::BGN){
                b= b&&e3();
            }
            else if(t->getTag()==Tag::DECL){
                b= b&&e4();
            }
            else if(t->getTag()==Tag::BODY){
                b= b&&e0();
            }
            else{
                erro("key-word 'begin' or 'declare'", t->getTag());
                b=false;
            }
            t = nextToken();
        }
        return b;
    }

    //S->program id body* $
    bool e0(){
        Token* t = nextToken();
        while(t->getTag()!=Tag::EoF){
            erro("End of File", t->getTag());
            t = nextToken();
        }
        return true;
    }

    //body->begin *stmt-lst end
    //stmt-lst-> *stmt; [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*do-while-stmt 
    //stmt->*shift-stmt 
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    //do-while-stmt->*do stmt-list stmt-suffix
    //shift-stmt->*id shiftop constant
    bool e3(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b= b&&e5(); 
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;

                case Tag::WHILE :
                    b= b&&e7();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;
            
                case Tag::STMTLST :
                    b= b&&e10();
                    return b;

                case Tag::STMT :
                    b= b&&e11();
                    break;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::DOWHILESTMT :
                    b= b&&e64();
                    break;

                case Tag::SHIFTSTMT :
                    b= b&&e65();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;

                case Tag::DO :
                    b= b&&e66();
                    break;
                
                default:
                    erro("identifier, key-words 'if', 'while', 'read' or 'write' ", t->getTag());
                    b=false;

            }
            
            t = nextToken();
        }
        return b;
    }

    //body->declare *decl-lst begin stmt-lst end
    //decl-lst->*decl; [decl;]
    //decl->*type ident-list
    //type->*integer
    //type->*decimal
    bool e4(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::INT :
                {
                    Token * n = new Token(Tag::TYPE);
                    addToken(n);
                    break;
                }
                case Tag::BODY :
                {
                    
                    addToken(t);
                    return b;;
                }

                case Tag::DEC :
                {
                    Token * n = new Token(Tag::TYPE);
                    addToken(n);
                    break;
                }

                case Tag::TYPE :
                    b=b&&e17();
                    break;

                case Tag::DECL :
                    b=b&&e18();
                    break;

                case Tag::DECLLST :
                    b=b&&e19();
                    return b;
                default:
                    b=false;
                    erro("key-word 'integer' or 'decimal'", t->getTag());
            }
            
            t = nextToken();
        }
        return b;
    }


    //assign-stmt->id *:= simple_expr
    //shift-stmt->id *shiftop constant
    //shiftop->*
    bool e5(){
        Token* t =nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::ATRIB){
                b= b&&e20();
                break;
            }
            erro("key-word ':='", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //if-stmt->if *condition then stmt-list end
    //if-stmt->if *condition then stmt-list else stmt-list end
    //condition->*expression
    //expression->*simple-expr 
    //expression->*simple-expr relop simple-expr
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    //num->*num
    //num->*num . num
    bool e6(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::CONDITION :
                    b= b&&e26();
                    return b;

                case Tag::EXPRESSION :
                    b=b&&e27();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e28();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;
                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            
            t = nextToken();
        }
        return b;
    }

    //while-stmt->while *condition do stmt-list end
    //condition->*expression
    //expression->*simple-expr 
    //expression->*simple-expr relop simple-expr
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e7(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::CONDITION :
                    b= b&&e26();
                    break;

                case Tag::EXPRESSION :
                    b=b&&e27();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e28();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            
            t = nextToken();
        }
        return b;
    }

    //read-stmt->read *( id )
    bool e8(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::OPAR){
                b= b&&e32();
                return b;
            }
            b=false;
            erro("key-word '('", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //write-stmt->write *( writable )
    bool e9(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::OPAR){
                b= b&&e33();
                break;
            }
            b=false;
            erro("key-word '('", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //body->begin stmt-lst* end
    bool e10(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::END){
                Token * n = new Token(Tag::BODY);
                addToken(n);
                return b;
            }
            b=false;
            erro("key-word 'end'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    
    
    
    //stmt-lst-> stmt*; [stmt;]
    bool e11(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::SMC){
                b=b&&e71();
                break;
            }
            b=false;
            erro("key-word ';'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //stmt->assign-stmt*
    bool e12(){
        Token* n = new Token(Tag::STMT);
        addToken(n);
        return true;
    }

    //stmt->if-stmt*
    bool e13(){
        Token* n = new Token(Tag::STMT);
        addToken(n);
        return true;
    }

    //stmt->while-stmt*
    bool e14(){
        Token* n = new Token(Tag::STMT);
        addToken(n);
        return true;
    }

    //stmt->read-stmt*
    bool e15(){
        Token* n = new Token(Tag::STMT);
        addToken(n);
        return true;
    }

    //stmt->write-stmt*
    bool e16(){
        Token* n = new Token(Tag::STMT);
        addToken(n);
        return true;
    }

    //decl->type *ident-list
    //ident-lst->*id {"," id}
    bool e17(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b=b&&e35();
                    break;

                case Tag::IDLST :
                {
                    Token* n = new Token(Tag::DECL);
                    addToken(n);
                    return b;
                }
                
                default:
                    erro("identifier", t->getTag());
                    b=false;

            }
            
            t = nextToken();
        }
        return b;
    }

    //decl-lst->decl*; [decl;]
    bool e18(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::SMC){
                b=b&&e37();
                return b;
            }
            b=false;
            erro("key-word ';'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //body->declare decl-lst *begin stmt-lst end
    bool e19(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::BGN){
                b=b&&e3();
                break;
            }
            b=false;
            erro("key-word 'begin'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //assign-stmt->id :=* simple_expr
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e20(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::SIMPLEEXPRESSION :
                {
                    b=b&&e73();
                    return b;
                
                }
                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;
                
                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            t = nextToken();
        }
        return b;
    }
    
    //simple-expr->"(" *simple-expr ")" ? simple-expr ":" simple-exp
    //factor->"(" *expression ")"
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    //expression->*simple-expr 
    //expression->*simple-expr relop simple-expr
    bool e21(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::EXPRESSION :
                    b=b&&e38();
                    return b;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e28();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            
            t = nextToken();
        }
        return b;
    }

    //factor-a->not *factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e22(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::ID :
                {
                    Token* n = new Token(Tag::FACTOR);
                    addToken(n);
                    break;
                }

                case Tag::NUM :
                {
                    Token* n = new Token(Tag::FACTOR);
                    addToken(n);
                    break;
                }

                case Tag::FACTOR :
                {
                    Token* n = new Token(Tag::FACTORA);
                    addToken(n);
                    return true;
                }

                default: 
                    erro("identifier, number or key-word '(' ", t->getTag());
                    b=false;
            }
            t = nextToken();
        }
        return b;
    }

    //factor-a-> - *factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e23(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::ID :
                {
                    Token* n = new Token(Tag::FACTOR);
                    addToken(n);
                    break;
                }

                case Tag::NUM :
                {
                    Token* n = new Token(Tag::FACTOR);
                    addToken(n);
                    break;
                }

                case Tag::FACTOR :
                {
                    Token* n = new Token(Tag::FACTORA);
                    addToken(n);
                    return true;
                }

                default: 
                    erro("identifier, number or key-word '(' ", t->getTag());
                    b=false;
            }
            t = nextToken();
        }
        return b;
    }

    //factor->id*
    bool e24(){
        Token* n = new Token(Tag::FACTOR);
        addToken(n);
        return true;
    }

    //factor->num*
    bool e25(){
        Token* n = new Token(Tag::FACTOR);
        addToken(n);
        return true;
    }

    //while-stmt->while condition *do stmt-list end
    //if-stmt->if condition *then stmt-list end
    //if-stmt->if condition *then stmt-list else stmt-list end
    bool e26(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::DO :
                    b= b&&e40();
                    return b;

                case Tag::THEN :
                    b= b&&e41();
                    return b;
                    
                default: 
                    erro("identifier, number or key-word '(' ", t->getTag());
                    b=false;
            }
            t = nextToken();
        }
        return b;
    }

    //condition->expression*
    bool e27(){
        Token* n = new Token(Tag::CONDITION);
        addToken(n);
        return true;
    }

    //expression->simple-expr* 
    //expression->simple-expr* relop simple-expr
    //simple-expr->simple-expr* addop term
    //simple-expr->"(" simple-expr* ")" ? simple-expr ":" simple-exp
    //relop->*<
    //relop->*>
    //relop->*<>
    //relop->*<=
    //relop->*>=
    //relop->*=
    //addop->*or
    //addop->*-
    //addop->*+
    bool e28(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::CPAR :
                    b= b&&e42();
                    break;

                case Tag::RELOP :
                    b= b&&e43();
                    return b;

                case Tag::ADDOP :
                    b= b&&e44();
                    break;

                case Tag::ADD :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                case Tag::SUB :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }
                
                case Tag::OR :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                case Tag::LT :
                {
                    Token* n = new Token(Tag::RELOP);
                    addToken(n);
                    break;
                }
                    
                case Tag::GT :
                {
                    Token* n = new Token(Tag::RELOP);
                    addToken(n);
                    break;
                }

                case Tag::NE :
                {
                    Token* n = new Token(Tag::RELOP);
                    addToken(n);
                    break;
                }

                case Tag::EQ :
                {
                    Token* n = new Token(Tag::RELOP);
                    addToken(n);
                    break;
                }

                default: 
                {
                    Token* n = new Token(Tag::EXPRESSION);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                    
            }
            t = nextToken();
        }
        return b;
    }

    //simple-expr->term*
    //term->term *mulop factor-a
    //mulop->*'*'
    //mulop->*'/'
    //mulop->*and
    //mulop->*mod
    bool e29(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::MULOP :
                    b= b&&e45();
                    return b;

                case Tag::MLT :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                case Tag::DIV :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }
                
                case Tag::AND :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                case Tag::MOD :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                default: 
                {
                    Token* n = new Token(Tag::SIMPLEEXPRESSION);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                    
            }
            t = nextToken();
        }
        return b;
    }

    //term->factor-a*
    bool e30(){
        Token* n = new Token(Tag::TERM);
        addToken(n);
        return true;
    }

    //factor-a->factor*
    bool e31(){
        Token* n = new Token(Tag::FACTORA);
        addToken(n);
        return true;
    }

    //read-stmt->read ( *id )
    bool e32(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::ID){
                b= b&&e46();
                return b;
            }
            b=false;
            erro("identifier", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //write-stmt->write ( *writable )
    //writable->*literal
    //writable->*simple-expr
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e33(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::WRITEABLE :
                    b= b&&e47();
                    return b;

                case Tag::LIT :
                {
                    Token* n = new Token(Tag::WRITEABLE);
                    addToken(n);
                    break;
                }

                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e67();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;
                
                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            t = nextToken();
        }
        return b;
    }

    //stmt-lst-> stmt;* [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    bool e34(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b= b&&e5();
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;
                    
                case Tag::STMTLST : 
                    return b;

                case Tag::WHILE :
                    b= b&&e7();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;

                case Tag::STMT :
                    b= b&&e11();
                    break;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;
                default:
                {
                    Token* n = new Token(Tag::STMTLST);
                    addToken(t);
                    addToken(n);
                    return b;
                }

            }
            t = nextToken();
        }
        return b;
    }

    //ident-lst->id* {"," id}
    bool e35(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::COM :
                    b= b&&e70();
                    return b;

                default: 
                {
                    Token* n = new Token(Tag::IDLST);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                    
            }
            t = nextToken();
        }
        return b;
    }

    //decl->type ident-list*
    bool e36(){
        Token* n = new Token(Tag::DECL);
        addToken(n);
        return true;
    }

    //decl-lst->decl;* [decl;]
    //decl->*type ident-list
    //type->*integer
    //type->*decimal
    bool e37(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::INT :
                {
                    Token * n = new Token(Tag::TYPE);
                    addToken(n);
                    break;
                }

                case Tag::DEC :
                {
                    Token * n = new Token(Tag::TYPE);
                    addToken(n);
                    break;
                }

                case Tag::TYPE :
                {
                    b=b&&e17();
                    break;
                }

                case Tag::DECL :
                {
                    b=b&&e18();
                    break;
                }

                case Tag::DECLLST :
                {
                    b=b&&e19();
                    return b;
                }
                
                default:
                {
                    Token* n = new Token(Tag::DECLLST);
                    addToken(t);
                    addToken(n);
                    return b;
                }
            }
            t = nextToken();
        }
        return b;
    }

    //factor->"(" expression *")"
    bool e38(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::CPAR){
                Token* n = new Token(Tag::FACTOR);
                addToken(n);
                return b;
            }
            b=false;
            erro("key-word ')'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //e39 -> e21

    //while-stmt->while condition do *stmt-list end
    //stmt-lst-> *stmt; [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    bool e40(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::STMTLST:
                    b=b&&e48();
                    break;
                case Tag::ID :
                    b= b&&e5();
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;

                case Tag::WHILE :
                    b= b&&e7();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;

                case Tag::STMT :
                    b= b&&e11();
                    break;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;
                default:
                    erro("identifier, key-words 'if', 'while', 'read' or 'write' ", t->getTag());
                    b=false;

            }
            
            t = nextToken();
        }
        return b;
    }

    //if-stmt->if condition then *stmt-list end
    //if-stmt->if condition then *stmt-list else stmt-list end
    //stmt-lst-> *stmt; [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    bool e41(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::STMTLST:
                    b=b&&e49();
                    return b;
                case Tag::ID :
                    b= b&&e5();
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;

                case Tag::WHILE :
                    b= b&&e7();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;

                case Tag::STMT :
                    b= b&&e11();
                    break;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;
                default:
                    erro("identifier, key-words 'if', 'while', 'read' or 'write' ", t->getTag());
                    b=false;

            }
            
            t = nextToken();
        }
        return b;
    }

    //simple-expr->"(" simple-expr ")" *? simple-expr ":" simple-exp
    bool e42(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::QST){
                b= b&&e50();
                return b;
            }
            b=false;
            erro("key-word '?'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //expression->simple-expr relop *simple-expr
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e43(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::SIMPLEEXPRESSION :
                {
                    Token* n = new Token(Tag::EXPRESSION);
                    addToken(n);
                    return b;
                }

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            
            t = nextToken();
        }
        return b;
    }

    //simple-expr->simple-expr addop *term
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e44(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e52();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;


                case Tag::TERM :
                    b=b&&e72();
                    return b;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            t = nextToken();
        }
        return b;
    }

    //term->term mulop *factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e45(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e52();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;


                case Tag::FACTORA :
                {
                    Token* n = new Token(Tag::TERM);
                    addToken(n);
                    return b;
                }
                    

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            t = nextToken();
        }
        return b;
    }

    //read-stmt->read ( id *)
    bool e46(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::CPAR){
                Token* n = new Token(Tag::READSTMT);
                addToken(n);
                return b;
            }
            b=false;
            erro("key-word ')'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //write-stmt->write ( writable *)
    bool e47(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::CPAR){
                Token* n = new Token(Tag::WRITESTMT);
                addToken(n);
                return b;
            }
            b=false;
            erro("key-word ')'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //while-stmt->while condition do stmt-list *end
    bool e48(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::END){
                Token* n = new Token(Tag::WHILESTMT);
                addToken(n);
                return b;
            }
            b=false;
            erro("key-word 'end'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //if-stmt->if condition then stmt-list *end
    //if-stmt->if condition then stmt-list *else stmt-list end
    bool e49(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::END){
                Token* n = new Token(Tag::IFSTMT);
                addToken(n);
                return b;
            }
            else if(t->getTag()==Tag::ELSE){
                b=b&&e55();
                break;
            }
            b=false;
            erro("key-word 'end' or 'else'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //simple-expr->"(" simple-expr ")" ? *simple-expr ":" simple-exp
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e50(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e56();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            t = nextToken();
        }
        return b;
    }

    //expression->simple-expr relop *simple-expr
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //simple-expr->*term
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e51(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e57();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;


                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            t = nextToken();
        }
        return b;
    }

    //factor->"(" *expression ")"
    //expression->*simple-expr 
    //expression->*simple-expr relop simple-expr
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //simple-expr->*term
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e52(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e59();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;
                
                case Tag::EXPRESSION :
                    b=b&&e58();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            
            t = nextToken();
        }
        return b;
    }

    //term->term *mulop factor-a
    //mulop->*'*'
    //mulop->*'/'
    //mulop->*and
    //mulop->*mo
    bool e53(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::MULOP :
                    b= b&&e60();
                    break;

                case Tag::MLT :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                case Tag::DIV :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }
                
                case Tag::AND :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                case Tag::MOD :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                default:
                    erro("'*', '/', 'and' or 'mod' ", t->getTag());
                    b=false;
                    
            }
            
            t = nextToken();
        }
        return b;
    }

    //term->term mulop factor-a*
    bool e54(){
        Token* n = new Token(Tag::TERM);
        addToken(n);
        return true;
    }

    //if-stmt->if condition then stmt-list else *stmt-list end
    //stmt-lst-> *stmt; [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    bool e55(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b= b&&e5();
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;

                case Tag::WHILE :
                    b= b&&e7();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;
            
                case Tag::STMTLST :
                    b= b&&e61();
                    return b;

                case Tag::STMT :
                    b= b&&e11();
                    break;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;
                
                default:
                    erro("identifier, key-words 'if', 'while', 'read' or 'write' ", t->getTag());
                    b=false;

            }
            
            t = nextToken();
        }
        return b;
    }

    //simple-expr->"(" simple-expr ")" ? simple-expr *":" simple-exp
    //simple-expr->simple-expr *addop term
    //addop->*or
    //addop->*-
    //addop->*+
    bool e56(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::CLM :
                    b= b&&e62();
                    break;

                case Tag::ADDOP :
                    b= b&&e44();
                    break;

                case Tag::ADD :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                case Tag::SUB :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }
                
                case Tag::OR :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                default:
                    erro("key-words ':', '+' , 'or' or '-'", t->getTag());
                    b=false;

                    
            }
            
            t = nextToken();
        }
        return b;
    }

    //expression->simple-expr relop simple-expr*
    //simple-expr->simple-expr *addop term
    //addop->*or
    //addop->*-
    //addop->*+
    bool e57(){
         Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ADDOP :
                    b= b&&e44();
                    break;

                case Tag::ADD :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                case Tag::SUB :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }
                
                case Tag::OR :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                default: 
                {
                    Token* n = new Token(Tag::EXPRESSION);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                    
            }
            t = nextToken();
        }
        return b;
    }

    //factor->"(" expression *")"
    bool e58(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::CPAR){
                Token* n = new Token(Tag::FACTOR);
                addToken(n);
                return b;
            }
            b=false;
            erro("')'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //expression->simple-expr* 
    //expression->*simple-expr relop simple-expr
    //simple-expr->*simple-expr addop term
    //e59 -> e28
    bool e59(){
        return e28();
    }

    //term->term mulop *factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"

    //e60 -> e45
    bool e60(){
        return e45();
    }

    //if-stmt->if condition then stmt-list else stmt-lst *end
    bool e61(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::END){
                Token* n = new Token(Tag::IFSTMT);
                addToken(n);
                return b;
            }
            b=false;
            erro("'end'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //simple-expr->"(" simple-expr ")" ? simple-expr ":" *simple-expr
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //simple-expr->*term
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e62(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e63();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            
            t = nextToken();
        }
        return b;
    }

    //simple-expr->"(" simple-expr ")" ? simple-expr ":" simple-expr*
    //simple-expr->simple-expr *addop term
    //addop->*or
    //addop->*-
    //addop->*+
    bool e63(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){

                case Tag::ADDOP :
                    b= b&&e44();
                    break;

                case Tag::ADD :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                case Tag::SUB :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }
                
                case Tag::OR :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                default:
                {
                    Token* n = new Token(Tag::SIMPLEEXPRESSION);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                    
            }
            erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //stmt->do-while-stmt*
    bool e64(){
        Token* n = new Token(Tag::STMT);
        addToken(n);
        return true;
    }

    //stmt->shift-stmt*
    bool e65(){
        Token* n = new Token(Tag::STMT);
        addToken(n);
        return true;
    }

    //do-while-stmt->do *stmt-list stmt-suffix
    //stmt-lst-> *stmt; [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    bool e66(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b= b&&e5();
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;

                case Tag::WHILE :
                    b= b&&e7();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;
            
                case Tag::STMTLST :
                    b= b&&e68();
                    return b;

                case Tag::STMT :
                    b= b&&e74();
                    break;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::DOWHILESTMT :
                    b= b&&e64();
                    break;

                case Tag::SHIFTSTMT :
                    b= b&&e65();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;

                case Tag::DO :
                    b= b&&e66();
                    break;

                default:
                    erro("identifier, key-words 'if', 'while', 'read' or 'write' ", t->getTag());
                    b=false;

            }
            t = nextToken();
        }
        return b;
    }

    //writable->simple-expr*
    //simple-expr->simple-expr *addop term
    //addop->*or
    //addop->*-
    //addop->*+
    bool e67(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){

                case Tag::ADDOP :
                    b= b&&e44();
                    break;

                case Tag::ADD :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                case Tag::SUB :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }
                
                case Tag::OR :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                default:
                {
                    Token* n = new Token(Tag::WRITEABLE);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                
            }
           erro("key-words ':', '+' , 'or' or '-'", t->getTag());
            t = nextToken();
        }
        return b;
    }

    //do-while-stmt->do stmt-list *stmt-suffix
    //stmt-suffix->*while condition
    bool e68(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::STMTSUFFIX :
                {
                    Token* n = new Token(Tag::DOWHILESTMT);
                    addToken(n);
                    return b;
                }

                case Tag::WHILE :
                    b= b&&e69();
                    break;

                default:
                    b=false;
                    erro("key-word 'while' ", t->getTag());

            }
            
            t = nextToken();
        }
        return b;
    }

    //stmt-suffix->while *condition
    //condition->*expression
    //expression->*simple-expr 
    //expression->*simple-expr relop simple-expr
    //simple-expr->*term
    //simple-expr->*simple-expr addop term
    //simple-expr->*"(" simple-expr ")" ? simple-expr ":" simple-exp
    //term->*factor-a
    //term->*term mulop factor-a
    //factor-a->*factor
    //factor-a->*not factor
    //factor-a->* - factor
    //factor->*id
    //factor->*num
    //factor->*"(" expression ")"
    bool e69(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::CONDITION :
                {
                    Token* n = new Token(Tag::STMTSUFFIX);
                    addToken(n);
                    return b;
                }

                case Tag::EXPRESSION :
                    b=b&&e27();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e28();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;
                
                default:
                    b=false;
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
            }
            
            t = nextToken();
        }
        return b;
    }

    //decl->type *ident-list
    //ident-lst->*id {"," id}
    bool e70(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b=b&&e35();
                    return b;

                case Tag::IDLST :
                {
                    Token* n = new Token(Tag::DECL);
                    addToken(n);
                    return b;
                }
                
                default:
                    erro("identifier", t->getTag());
                    b=false;

            }
            
            t = nextToken();
        }
        return b;
    }
    
    //body->begin *stmt-lst end
    //stmt-lst-> *stmt; [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*do-while-stmt 
    //stmt->*shift-stmt 
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    //do-while-stmt->*do stmt-list stmt-suffix
    //shift-stmt->*id shiftop constant
    bool e71(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b= b&&e5(); 
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;

                case Tag::WHILE :
                    b= b&&e7();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;
            
                case Tag::STMTLST :
                    b= b&&e10();
                    return b;

                case Tag::STMT :
                    b= b&&e11();
                    return b;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::DOWHILESTMT :
                    b= b&&e64();
                    break;

                case Tag::SHIFTSTMT :
                    b= b&&e65();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;

                case Tag::DO :
                    b= b&&e66();
                    break;
                
                default:
                {
                    Token*n = new Token(Tag::STMTLST);
                    addToken(t);
                    addToken(n);
                    return b;
                }

            }
            
            t = nextToken();
        }
        return b;
    }
    
    //simple-expr->simple-expr addop term*
    //term->term *mulop factor-a
    //mulop->*'*'
    //mulop->*'/'
    //mulop->*and
    //mulop->*mod
    bool e72(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::MULOP :
                    b= b&&e45();
                    return b;

                case Tag::MLT :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                case Tag::DIV :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }
                
                case Tag::AND :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                case Tag::MOD :
                {
                    Token* n = new Token(Tag::MULOP);
                    addToken(n);
                    break;
                }

                default: 
                {
                    Token* n = new Token(Tag::SIMPLEEXPRESSION);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                    
            }
            t = nextToken();
        }
        return b;
    }
    
    //assign-stmt->id := simple_expr*
    //simple-expr->simple-expr *addop term
    bool e73(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){

                case Tag::ADDOP :
                {
                    b= b&&e44();
                    Token* t = nextToken();
                    Token* n = new Token(Tag::ASSIGNSTMT);
                    addToken(n);
                    return b;
                }
                    
                    

                case Tag::ADD :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                case Tag::SUB :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }
                
                case Tag::OR :
                {
                    Token* n = new Token(Tag::ADDOP);
                    addToken(n);
                    break;
                }

                default:
                {
                    Token* n = new Token(Tag::ASSIGNSTMT);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                
            }
            t = nextToken();
        }
        return b;
    }
    
    //stmt-lst-> stmt*; [stmt;]
    bool e74(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            if(t->getTag()==Tag::SMC){
                b=b&&e75();
                break;
            }
            b=false;
            erro("key-word ';'", t->getTag());
            t = nextToken();
        }
        return b;
    }
    
    //body->begin *stmt-lst end
    //stmt-lst-> *stmt; [stmt;]
    //stmt->*assign-stmt
    //stmt->*if-stmt
    //stmt->*while-stmt
    //stmt->*read-stmt
    //stmt->*do-while-stmt 
    //stmt->*shift-stmt 
    //stmt->*write-stmt 
    //assign-stmt->*id := simple_expr
    //if-stmt->*if condition then stmt-list end
    //if-stmt->*if condition then stmt-list else stmt-list end
    //while-stmt->*while condition do stmt-list end
    //read-stmt->*read ( id )
    //write-stmt->*write ( writable )
    //do-while-stmt->*do stmt-list stmt-suffix
    //shift-stmt->*id shiftop constant
    bool e75(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::ID :
                    b= b&&e5(); 
                    break;

                case Tag::IF : 
                    b= b&&e6();
                    break;

                case Tag::WHILE :
                    b= b&&e76();
                    break;
            
                case Tag::READ :
                    b= b&&e8();
                    break;
            
                case Tag::WRT :
                    b= b&&e9();
                    break;
            
                case Tag::STMTLST :
                    b= b&&e10();
                    return b;

                case Tag::STMT :
                    b= b&&e11();
                    return b;
            
                case Tag::ASSIGNSTMT :
                    b= b&&e12();
                    break;
            
                case Tag::IFSTMT :
                    b= b&&e13();
                    break;

                case Tag::WHILESTMT :
                    b= b&&e14();
                    break;

                case Tag::DOWHILESTMT :
                    b= b&&e64();
                    break;

                case Tag::SHIFTSTMT :
                    b= b&&e65();
                    break;

                case Tag::READSTMT :
                    b= b&&e15();
                    break;
            
                case Tag::WRITESTMT :
                    b= b&&e16();
                    break;

                case Tag::DO :
                    b= b&&e66();
                    break;
                
                default:
                {
                    Token*n = new Token(Tag::STMTLST);
                    addToken(t);
                    addToken(n);
                    return b;
                }

            }
            
            t = nextToken();
        }
        return b;
    }
    
    
    //while-stmt->while *condition do stmt-list end
    //stmt-suffix->while *condition
    bool e76(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){
                case Tag::OPAR :
                    b= b&&e21();
                    break;

                case Tag::NOT :
                    b= b&&e22();
                    break;

                case Tag::SUB :
                    b= b&&e23();
                    break;

                case Tag::ID :
                    b= b&&e24();
                    break;

                case Tag::NUM :
                    b= b&&e25();
                    break;

                case Tag::CONDITION :
                    b= b&&e78();
                    return b;

                case Tag::EXPRESSION :
                    b=b&&e27();
                    break;

                case Tag::SIMPLEEXPRESSION :
                    b=b&&e28();
                    break;

                case Tag::TERM :
                    b=b&&e29();
                    break;

                case Tag::FACTORA :
                    b=b&&e30();
                    break;

                case Tag::FACTOR :
                    b=b&&e31();
                    break;

                default:
                    erro("identifier, number, key-words '(' , 'not' or '-'", t->getTag());
                    b=false;
            }

            
            t = nextToken();
        }
        return b;
    }

    //while-stmt->while condition *do stmt-list end
    //stmt-suffix->while condition*
    bool e78(){
        Token* t = nextToken();
        bool b=true;
        while(t->getTag()!=Tag::EoF){
            switch(t->getTag()){

                case Tag::DO :
                {
                    b= b&&e40();
                    return b;
                }
                    
                default:
                {
                    Token* n = new Token(Tag::STMTSUFFIX);
                    addToken(t);
                    addToken(n);
                    return b;
                }
                
            }
            t = nextToken();
        }
        return b;
    }
};