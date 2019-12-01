#ifndef CLASSES_H
#define CLASSES_H 

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum Obj{
        PROGRAM, CONSTANT, GLOBALVARIABLE, FUNCTION, PARAMETER, VARIABLE, DOWHILE, IF, WHILE, FOR, PRINTF, SCANF, EXIT, RETURN, OPERATION, OPRESULT, OPLEAF, OP_ADD,OP_SUB,OP_MUL,OP_DIV,OP_VARIABLE,OP_CONSTANT,OP_FUNCTION,OP_ASSIGN,INT,CHAR,LOGICAL_AND,LOGICAL_OR, GREATER_THAN,LESS_THAN,EQUAL,NOT_EQUAL,LESS_EQUAL,GREATER_EQUAL,ARITHMETICAL,LOGICAL
};



int getNextRegister(bool reg[]){
    int r;
    for(r = 0; r<10;r++){
        if(!reg[r]){
            reg[r]=true;
            break;
        }        
    }
    return r;
}


class container{
    public:
        int type;
        void *obj;
};


class OpLeaf { //folha da arvore de operacoes, pode ser qualquer coisa
    public:
        int getObjType = OPLEAF;
        int type; //OP_VARIABLE, OP_CONSTANT,OP_FUNCTION
        int valueType; // char / int
        std::string valueId; //caso variavel = id, caso constante = valor, caso funcao = id;
        std::vector<std::string> values;
        
};

class Operation {
    public:
        int objType = OPERATION;
        int opGroup;// operacao (artimetica/logica)
        int opType; //tipo de operacao
        container *left; //filho a esquerda, caso operacao unaria, eh o filho unico
        container *right; //filho a direita

        OpLeaf* print(bool reg[]){
            OpLeaf *leftLeaf,*rightLeaf;
            Operation *leftOp,*rightOp;
            int r = getNextRegister(reg);
            int c1 = -1;
            int c2 = -1;

            if(left->type==OPLEAF){

                leftLeaf = (OpLeaf*)left->obj;
                if(leftLeaf->type==CONSTANT){
                    c1= getNextRegister(reg);
                    std::cout<<"addi $t"<<c2<<", $zero, "<<leftLeaf->valueId<<std::endl;
                    leftLeaf->type = OP_VARIABLE;
                    leftLeaf->valueType = INT;
                    leftLeaf->valueId = "t"+to_string(c2);

                }
            }else{
                leftOp = (Operation*)left->obj;
                leftLeaf = leftOp->print(reg);
            }
            if(right->type==OPLEAF){
                rightLeaf = (OpLeaf*)right->obj;

                if(rightLeaf->type==CONSTANT){
                    c2= getNextRegister(reg);
                    std::cout<<"addi $t"<<c2<<", $zero, "<<rightLeaf->valueId<<std::endl;
                    rightLeaf->type = OP_VARIABLE;
                    rightLeaf->valueType = INT;
                    rightLeaf->valueId = "t"+to_string(c2);
                }
            }else{
                rightOp = (Operation*)right->obj;
                rightLeaf = rightOp->print(reg);
            }
            OpLeaf *op = new OpLeaf();
                op->type = OP_VARIABLE;
                op->valueType = INT;
                op->valueId = "t"+to_string(r);
            
            cout<<opType<<endl;
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
                case OP_ASSIGN:
                std::cout<<"add $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<",$zero"<<std::endl;
                break;
            }
            if(c1!=-1)
                reg[c1]=false;
            if(c2!=-1)
                reg[c2]=false;
            reg[r]=false;
            return op;
        }

        bool printLogicalOperation(bool reg[],string labelTrue,string labelFalse,bool demorgan){
            OpLeaf *leftLeaf,*rightLeaf;
            Operation *leftOp,*rightOp;

            if(right==NULL){
                leftLeaf = (OpLeaf*)left->obj;
                std::cout<<"beq $"<<leftLeaf->valueId<<",$zero"<<labelTrue<<endl;
                return false;
            }
            int r = getNextRegister(reg);
            int c1 = -1;
            int c2 = -1;

            if(opType==LOGICAL_AND)
                demorgan=true;
            else if (opType == LOGICAL_OR)
                demorgan=false;
            

            if(left->type==OPLEAF){
                leftLeaf = (OpLeaf*)left->obj;
                if(leftLeaf->type==CONSTANT){
                    c1= getNextRegister(reg);
                    std::cout<<"addi $t"<<c2<<", $zero, "<<leftLeaf->valueId<<std::endl;
                    leftLeaf->type = OP_VARIABLE;
                    leftLeaf->valueType = INT;
                    leftLeaf->valueId = "t"+to_string(c2);
                }            
                }else{
                leftOp = (Operation*)left->obj;
                if(leftOp->opGroup==ARITHMETICAL){
                    leftLeaf = leftOp->print(reg);
                }else{
                    leftOp->printLogicalOperation(reg,labelTrue,labelFalse,demorgan);
                }
            }
            
            if(right->type==OPLEAF){
                rightLeaf = (OpLeaf*)right->obj;
                if(rightLeaf->type==CONSTANT){
                    c2= getNextRegister(reg);
                    std::cout<<"addi $t"<<c2<<", $zero, "<<rightLeaf->valueId<<std::endl;
                    rightLeaf->type = OP_VARIABLE;
                    rightLeaf->valueType = INT;
                    rightLeaf->valueId = "t"+to_string(c2);
                }    
            }else{
                rightOp = (Operation*)right->obj;
                if(rightOp->opGroup==ARITHMETICAL){
                  rightLeaf = rightOp->print(reg);
                }else{
                    rightOp->printLogicalOperation(reg,labelTrue,labelFalse,demorgan);
                }

            }
            

            switch(opType){
                case GREATER_THAN:
                    if(!demorgan){
                        std::cout<<"slt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                        std::cout<<"bne $t"<<r<<" ,$zero, "<<labelTrue<<endl;
                    }else{
                        std::cout<<"slt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                        std::cout<<"beq $t"<<r<<" ,$zero, "<<labelFalse<<endl;
                    }
                break;
                case LESS_THAN:
                    if(!demorgan){
                        std::cout<<"slt $t"<<r<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<endl;
                        std::cout<<"bne $t"<<r<<"$zero, "<<labelTrue<<endl;
                    }else{
                        std::cout<<"slt $t"<<r<<" ,$"<<leftLeaf->valueId<<" ,$"<<rightLeaf->valueId<<endl;
                        std::cout<<"beq $t"<<r<<" ,$zero, "<<labelFalse<<endl;
                    }
                break;
                case EQUAL:
                    if(!demorgan){
                        std::cout<<"beq $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelTrue<<endl;
                    }else{
                        std::cout<<"bne $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelFalse<<endl;
                    }
                break;
                case NOT_EQUAL:
                    if(!demorgan){
                        std::cout<<"bne $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelTrue<<endl;
                    }else{
                        std::cout<<"beq $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelFalse<<endl;
                    }
                break;
                case LESS_EQUAL:
                    if(!demorgan){
                        std::cout<<"slt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                        std::cout<<"beq $t"<<r<<" ,$zero, "<<labelTrue;
                    }else{
                        std::cout<<"slt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                        std::cout<<"bne $t"<<r<<" ,$zero, "<<labelFalse<<endl;
                    }

                break;
                case GREATER_EQUAL:
                    if(!demorgan){
                        std::cout<<"slt $t"<<r<<" ,$"<<leftLeaf->valueId<<" ,$"<<rightLeaf->valueId<<endl;
                        std::cout<<"beq $t"<<r<<" ,$zero, "<<labelTrue<<endl;
                    }else{
                        std::cout<<"slt $t"<<r<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<endl;
                        std::cout<<"bne $t"<<r<<"$zero, "<<labelFalse<<endl;
                    }
                break;
               
            }
            reg[r]=false;
            if(c1!=-1)
                reg[c1]=false;
            if(c2!=-1)
                reg[c2]=false;
            return demorgan;
        }
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
        void print(bool reg[]){
            Operation *op = (Operation*)condition->obj;
            string lblTrue = "do_while";
            string lblFalse = "end_do_while";
            cout<<lblTrue<<":"<<endl;
            cout<<"\n\n...\n\n"<<endl;
            bool t = op->printLogicalOperation(reg,lblTrue,lblFalse,false);
            // se não for AND 
            if(!t){
                cout<<"j "<<lblFalse<<endl;
            }else{
                std::cout<<"j "<<lblTrue<<endl;
            }
            std::cout<<lblFalse<<endl;
            
        }
};

class If {
    public:
        int objType = IF;
        std::vector<container*> exp; //expressao booleana
        std::vector<container*> then; //lista de comandos then
        std::vector<container*> els; //lista de comandos else
        void print(bool reg[]){
            container *ct = exp.front();
            Operation *op = (Operation*)ct->obj;
            bool t = op->printLogicalOperation(reg,"then","else",false);
            // se não for AND 
            if(!t){
                cout<<"j else"<<endl;
            }
            std::cout<<"then:"<<endl;
            std::cout<<"j fim"<<endl;
            std::cout<<"else:"<<endl;
            std::cout<<"fim:"<<endl;
            
        }
};

class While {
    public:
        int objType = WHILE;
        container *condition; //condicao de parada
        std::vector<container*> commands; //lista de comandos
        void print(bool reg[]){
            Operation *op = (Operation*)condition->obj;
            string lblTrue = "end_while";
            string lblFalse = "while";
            cout<<lblFalse<<":"<<endl;
            bool t = op->printLogicalOperation(reg,lblTrue,lblFalse,false);
            // se não for AND 
            cout<<"\n\n...\n\n"<<endl;
            if(!t){
                cout<<"j "<<lblFalse<<endl;
            }else{
                std::cout<<"j "<<lblTrue<<endl;
            }
            std::cout<<lblFalse<<endl;
            
        }
};

class For {
    public:
        int objType = FOR;
        container *init; //expressao de inicializacao
        container *condition; //condicao de parada
        container *adjustment; //ajuste
        std::vector<container*> commands; //lista de comandos
        void print(bool reg[]){
            Operation *opInit = (Operation*)init->obj;
            opInit->print(reg);

            string lblTrue = "end_for";
            string lblFalse = "for";
            cout<<lblFalse<<":"<<endl;
            Operation *opCondition = (Operation*)condition->obj;
            opCondition->printLogicalOperation(reg,lblTrue,lblFalse,false);
            cout<<"\n\n...\n\n"<<endl;

            Operation *adjust = (Operation*)adjustment->obj;
            adjust->print(reg);
            cout<<" j for"<<endl;

        }
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



class OpResult {
    public:
        int objType = OPRESULT; 
        std::string reg; //registrador
        std::string type; //tipo do valor
        std::string value; //valor do resultado
};


#endif