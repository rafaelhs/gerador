#ifndef CLASSES_H
#define CLASSES_H 

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum Obj{
        PROGRAM, CONSTANT, GLOBALVARIABLE, FUNCTION, PARAMETER, VARIABLE, DOWHILE, IF, WHILE, FOR, PRINTF, SCANF, EXIT, RETURN, OPERATION, OPRESULT, OPLEAF, OP_ADD,OP_SUB,OP_MUL,OP_DIV,OP_VARIABLE,OP_CONSTANT,OP_FUNCTION,INT,CHAR
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
        std::vector<container*> param; //lista de parametros
        std::string id; //identificador da funcao
        std::string return_type; //tipo de retorno
        std::vector<container*> commands; //lista de comandos

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
        std::vector<container*> commands; //lista de comandos
        container *condition; //condicao de parada
};

class If {
    public:
        int objType = IF;
        std::vector<container*> exp; //expressao booleana
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
class OpLeaf { //folha da arvore de operacoes, pode ser qualquer coisa
    public:
        int getObjType = OPLEAF;
        int type; //OP_VARIABLE, OP_CONSTANT,OP_FUNCTION
        int valueType; // char / int
        std::string valueId; //nome do registrador
        std::vector<std::string> values; //caso variavel = id, caso constante = valor, caso funcao = parametros;
        
};
class Operation {
    public:
        int objType = OPERATION;
        int opType; //tipo de operacao
        container *left; //filho a esquerda, caso operacao unaria, eh o filho unico
        container *right; //filho a direita
        OpLeaf* print(){
            OpLeaf *leftLeaf;
            OpLeaf *rightLeaf;
            
            Operation *leftOp;
            Operation *rightOp;
            
            if(left->type==OPLEAF){
                leftLeaf = (OpLeaf*)left->obj;
            }else{
                leftOp = (Operation*)left->obj;
                leftLeaf = leftOp->print();
            }
            if(right->type==OPLEAF){
                rightLeaf = (OpLeaf*)right->obj;
            }else{
                rightOp = (Operation*)right->obj;
                rightLeaf = rightOp->print();
            }
            
            OpLeaf *op = new OpLeaf();
                op->type = OP_VARIABLE;
                op->valueType = OP_VARIABLE;
                op->valueId = "t0";
            switch(opType){
                case OP_ADD:
                std::cout<<"add $"<<op->valueId<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
                break;
                case OP_SUB:
                std::cout<<"sub $"<<op->valueId<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
                break;
                case OP_MUL:
                std::cout<<"mul $"<<op->valueId<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
                break;
                case OP_DIV:
                std::cout<<"div $"<<op->valueId<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
                break;
            }

        return op;
        }
        
};




class OpResult {
    public:
        int objType = OPRESULT; 
        std::string reg; //registrador
        std::string type; //tipo do valor
        std::string value; //valor do resultado
};



#endif