#include <iostream>
#include <string>
#include <vector>
#include "classes.h"
using namespace std;


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

OpLeaf* Operation::print(bool reg[]){
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
bool Operation::printLogicalOperation(bool reg[],string labelTrue,string labelFalse,bool demorgan){
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



void DoWhile::print(bool reg[]){
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


void If::print(bool reg[]){
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

void For::print(bool reg[]){
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


void While::print(bool reg[]){
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