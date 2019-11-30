#ifndef CLASSES_H
#define CLASSES_H 

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum Obj{
        PROGRAM, CONSTANT, GLOBALVARIABLE, FUNCTION, PARAMETER, VARIABLE, DOWHILE, IF, WHILE, FOR, PRINTF, SCANF, EXIT, RETURN, OPERATION, OPRESULT, OPLEAF
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
        std::string type; //variavel = 0 , valor constante = 1  ou funcao = 2
        std::string valueType; //Tipo do valor
        std::string valueId; //nome da variavel
        std::vector<std::string> values; //caso variavel = id, caso constante = valor, caso funcao = parametros;
        
};
class Operation {
    public:
        int objType = OPERATION;
        int opType; //tipo de operacao
        container *left; //filho a esquerda, caso operacao unaria, eh o filho unico
        container *right; //filho a direita
        void print(){
            OpLeaf *leftLeaf;
            OpLeaf *rightLeaf;
            
            Operation *leftOp;
            Operation *rightOp;
            
            if(left->type==OPLEAF){
                leftLeaf = (OpLeaf*)left->obj;
            }else{
                leftOp = (Operation*)left->obj;
                leftLeaf = leftOp->avaliar();
            }
            if(right->type==OPLEAF){
                rightLeaf = (OpLeaf*)right->obj;
            }else{
                rightOp = (Operation*)right->obj;
                rightLeaf = rightOp->avaliar();
            }
            if(opType==0){//ADICAO
                std::cout<<"add $t,$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }else if(opType==1){//Subtracao
                std::cout<<"sub $t,$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }else if(opType==2){//multiplicacao
                std::cout<<"mul $t,$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }else if(opType==3){//divisao
                std::cout<<"div $t,$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }


        }
        OpLeaf* avaliar(){
            OpLeaf *leftLeaf;
            OpLeaf *rightLeaf;
            
            Operation *leftOp;
            Operation *rightOp;
            
            if(left->type==OPLEAF){
                leftLeaf = (OpLeaf*)left->obj;
            }else{
                leftOp = (Operation*)left->obj;
                leftLeaf = leftOp->avaliar();
            }
            if(right->type==OPLEAF){
                rightLeaf = (OpLeaf*)right->obj;
            }else{
                rightOp = (Operation*)right->obj;
                rightLeaf = rightOp->avaliar();
            }
            OpLeaf *op = new OpLeaf();
                op->type = "v";
                op->valueType = "int";
                op->valueId = "t";
            if(opType==0){//ADICAO
                std::cout<<"add $"<<op->valueId<<"$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }else if(opType==1){//Subtracao
                std::cout<<"sub $"<<op->valueId<<"$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }else if(opType==2){//multiplicacao
                std::cout<<"mul $"<<op->valueId<<"$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }else if(opType==3){//divisao
                std::cout<<"div $"<<op->valueId<<"$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<std::endl;
            }
            return op;
        }
};

<<<<<<< HEAD
//TODO
class OpLeaf { //folha da arvore de operacoes, pode ser qualquer coisa
    public:
        int ObjType = OPLEAF;
        std::string type; //variavel, valor constante ou funcao
        std::string valueType; //Tipo do valor
        std::string functionName; //

        std::vector<std::string> values; //caso variavel = id, caso constante = valor, caso funcao = nome funcao;

};
=======


>>>>>>> 5a05eb270b9b04eea01efe4ea0f8634b2808fe66

class OpResult {
    public:
        int objType = OPRESULT; 
        std::string reg; //registrador
        std::string type; //tipo do valor
        std::string value; //valor do resultado
};



#endif