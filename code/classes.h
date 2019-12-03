#ifndef CLASSES_H
#define CLASSES_H 

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum Obj{
        PROGRAM, CONSTANT, GLOBALVARIABLE, FUNCTION, PARAMETER, VARIABLE, DOWHILE, IF, WHILE, FOR, PRINTF, SCANF, EXIT, RETURN, OPERATION, OPRESULT, OPLEAF,
        OP_ADD,OP_SUB,OP_MUL,OP_DIV,OP_VARIABLE,OP_CONSTANT,OP_FUNCTION,OP_ASSIGN,INT,CHAR,LOGICAL_AND,LOGICAL_OR, GREATER_THAN,LESS_THAN,EQUAL,NOT_EQUAL,LESS_EQUAL,GREATER_EQUAL,REF,ARITHMETICAL,LOGICAL,OP_TEMPORARY
};

class container{
    public:
        int type;
        void *obj;
};


class OpLeaf { //folha da arvore de operacoes, pode ser qualquer coisa
    public:
        int getObjType = OPLEAF;
        int type; //OP_VARIABLE, OP_CONSTANT,OP_FUNCTION,OP_TEMPORARY
        int valueType; // char / int
        int regTemp;
        std::string valueId; //caso variavel = id, caso constante = valor, caso funcao = id;
        std::vector<container*> values;
        OpLeaf* printFunction();
        void printParameters();

};

class Operation {
    public:
        int objType = OPERATION;
        int opGroup;// operacao (artimetica/logica)
        int opType; //tipo de operacao
        container *left; //filho a esquerda, caso operacao unaria, eh o filho unico
        container *right; //filho a direita

        OpLeaf* print();
        OpLeaf* evalArithmeticLeaf(container *c);
        bool printLogicalOperation(string labelTrue,string labelFalse,bool demorgan);
        OpLeaf* evalLogicalLeaf(container *c,string labelTrue,string labelFalse,bool demorgan);
        int verifyGroup();

};

class Program {
    public:
        int objType = PROGRAM;
        std::map<std::string, container*> symbTable; //tabela de simbolos global
        std::vector<container*> printF; //lista de printfs
        std::vector<container*> arrays; //lista de vetores
        std::vector<container*> functions; //lista de funcoes
        void print();
        void printPrintf();
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

class RegisterData{
    public:
        std::map<std::string, std::string> regName; // map de conversão: variavel -> registrador
        bool tempRegisters[10]; //lista de uso de registradores
        string addReg(string name);
        int getNextRegister();
        void clearRegister(int r);
        std::string getReg(string name);
        void stackUp();
        void unstack();
};

class Function {
    public:
        int objType = FUNCTION;
        std::map<std::string, container*> symbTable; //tabela de simbolos da funcao
        std::vector<container*> param; //lista de parametros
        std::string id; //identificador da funcao
        std::string return_type; //tipo de retorno
        std::vector<container*> commands; //lista de comandos
        void print();
        void evalSymbTable();
        void printParam();

};

class Parameter {
    public:
        int objType = PARAMETER;
        std:: string id; //nome do parametro
        std:: string type; //tipo do parametro
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
        int idLabel; //identificador
        std::vector<container*> commands; //lista de comandos
        container *condition; //condicao de parada
        void print();
};

class If {
    public:
        int objType = IF;
        int idLabel; //identificador
        std::vector<container*> exp; //expressao booleana
        std::vector<container*> then; //lista de comandos then
        std::vector<container*> els; //lista de comandos else
        void print();
};

class While {
    public:
        int objType = WHILE;
        int idLabel; //identificador
        container *condition; //condicao de parada
        std::vector<container*> commands; //lista de comandos
        void print();
};

class For {
    public:
        int objType = FOR;
        int idLabel; //identificador
        container *init; //expressao de inicializacao
        container *condition; //condicao de parada
        container *adjustment; //ajuste
        std::vector<container*> commands; //lista de comandos
        void print();
};

class Printf {
    public:
        int objType = PRINTF;
        int idLabel; //identificador
        std::string str; //texto do printf
        std::vector<std::string> dataLabels;
        std::vector<std::string> labelValues;
        std::vector<container*> exp; //lista de expressoes
        void print();
        void printLabel();
};

class Scanf {
    public:
        int objType = SCANF; 
        std::string str; //texto do scanf
        container *address; //endereco
        void print();
};

class Exit {
    public:
        int objType = EXIT;
        container *exp; //expressao
        void print();
};

class Return {
    public:
        int objType = RETURN;
        container *exp; //expressao
        void print();
};

class OpResult {
    public:
        int objType = OPRESULT; 
        std::string reg; //registrador
        std::string type; //tipo do valor
        std::string value; //valor do resultado
};


#endif