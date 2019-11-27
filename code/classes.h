#ifndef CLASSES_H
#define CLASSES_H 

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum Obj{
        PROGRAM, CONSTANT, GLOBALVARIABLE, FUNCTION, VARIABLE, DOWHILE, IF, WHILE, FOR, PRINTF, SCANF, EXIT, RETURN, BOP, UOP
};


class container{
    public:
        int type;
        void *obj;
};

class Program {
    public:
        int objType = PROGRAM;
        std::map<std::string, container*> symbTable; //tabela de simbolos global
        std::vector<container*> printF; //lista de printfs
        std::vector<container*> arrays; //lista de vetores
        std::vector<container*> functions; //lista de funcoes
};

class Constant {
    public:
        int objType = CONSTANT;
        std::string id; //identificador da constante
        std::string value; //valor da constante
};

class GlobalVariable {
    public:
        int objType = GLOBALVARIABLE;
        std::string id; //identificador da variavel
        std::string type; //tipo da variavel
};

class Function {
    public:
        int objType = FUNCTION;
        std::map<std::string, container*> symbTable; //tabela de simbolos da funcao
        std::string id; //identificador da funcao
        std::string return_type; //tipo de retorno
        std::vector<container*> commands; //lista de comandos

};

class Variable {
    public:
        int objType = VARIABLE;
        std::string id; //identificador da variavel
        std::string type; //tipo da variavel
};

class DoWhile {
    public:
        int objType = DOWHILE;
        std::vector<container*> commands; //lista de comandos
        container *condition; //condicao de parada
};

class If {
    public:
        int objType = IF;
        container *exp; //expressao booleana
        std::vector<container*> then; //lista de comandos then
        std::vector<container*> els; //lista de comandos else
};

class While {
    public:
        int objType = WHILE;
        container *condition; //condicao de parada
        std::vector<container*> commands; //lista de comandos
};

class For {
    public:
        int objType = FOR;
        container *init; //expressao de inicializacao
        container *condition; //condicao de parada
        container *adjustment; //ajuste
        std::vector<container*> commands; //lista de comandos
};

class Printf {
    public:
        int objType = PRINTF;
        std::string str; //texto do printf
        std::vector<container*> exp; //lista de expressoes
};

class Scanf {
    public:
        int objType = SCANF; 
        std::string str; //texto do scanf
        container *address; //endereco
};

class Exit {
    public:
        int objType = EXIT;
        container *exp; //expressao
};

class Return {
    public:
        int objType = RETURN;
        container *exp; //expressao
};

class Bop {
    public:
        int objType = BOP;
        int opType; //tipo de operacao
        container *left; //filho a esquerda
        container *right; //filho a direita
};

class Uop {
    public:
        int objType = UOP;
        int opType; //tipo de operacao
        container *exp; //expressao
};
#endif