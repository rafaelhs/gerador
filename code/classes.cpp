#include <iostream>
#include <string>
#include <vector>
#include "classes.h"
using namespace std;

RegisterData *rData;

void descricao(int d){
    switch(d){
        case PROGRAM: cout<<"\tPROGRAM"<<endl;        return;
        case CONSTANT: cout<<"\tCONSTANT"<<endl;              return;
        case GLOBALVARIABLE: cout<<"\tGLOBALVARIABLE"<<endl;  return;
        case FUNCTION: cout<<"\tFUNCTION"<<endl;              return;
        case PARAMETER: cout<<"\tPARAMETER"<<endl;            return;
        case VARIABLE: cout<<"\tVARIABLE"<<endl;              return;
        case DOWHILE: cout<<"\tDOWHILE"<<endl;                return;
        case IF: cout<<"\tIF"<<endl;                          return;
        case WHILE: cout<<"\tWHILE"<<endl;                    return;
        case FOR: cout<<"\tFOR"<<endl;                        return;
        case PRINTF: cout<<"\tPRINTF"<<endl;                  return;
        case SCANF: cout<<"\tSCANF"<<endl;                    return;
        case EXIT: cout<<"\tEXIT"<<endl;                      return;
        case RETURN: cout<<"\tRETURN"<<endl;                  return;
        case OPERATION: cout<<"\tOPERATION"<<endl;            return;
        case OPRESULT: cout<<"\tOPRESULT"<<endl;              return;
        case OPLEAF: cout<<"\tOPLEAF"<<endl;                  return;
        case OP_ADD: cout<<"\tOP_ADD"<<endl;                  return;
        case OP_SUB: cout<<"\tOP_SUB"<<endl;                  return;
        case OP_MUL: cout<<"\tOP_MUL"<<endl;                  return;
        case OP_DIV: cout<<"\tOP_DIV"<<endl;                  return;
        case OP_VARIABLE: cout<<"\tOP_VARIABLE"<<endl;        return;
        case OP_CONSTANT: cout<<"\tOP_CONSTANT"<<endl;        return;
        case OP_FUNCTION: cout<<"\tOP_FUNCTION"<<endl;        return;
        case OP_ASSIGN: cout<<"\tOP_ASSIGN"<<endl;            return;
        case INT: cout<<"\tINT"<<endl;                        return;
        case CHAR: cout<<"\tCHAR"<<endl;                      return;
        case LOGICAL_AND: cout<<"\tLOGICAL_AND"<<endl;        return;
        case LOGICAL_OR: cout<<"\tLOGICAL_OR"<<endl;          return;
        case GREATER_THAN: cout<<"\tGREATER_THAN"<<endl;      return;
        case LESS_THAN: cout<<"\tLESS_THAN"<<endl;            return;
        case EQUAL: cout<<"\tEQUAL"<<endl;                    return;
        case NOT_EQUAL: cout<<"\tNOT_EQUAL"<<endl;            return;
        case LESS_EQUAL: cout<<"\tLESS_EQUAL"<<endl;          return;
        case GREATER_EQUAL: cout<<"\tGREATER_EQUAL"<<endl;    return;
        case ARITHMETICAL: cout<<"\tARITHMETICAL"<<endl;      return;
        case LOGICAL: cout<<"\tLOGICAL"<<endl;                return;
        case OP_TEMPORARY: cout<<"\tOP_TEMPORARY"<<endl;      return;
    }  
}

void printObj(container *c){
    switch(c->type){

        case OPERATION: ((Operation*)c->obj)->print();
        case DOWHILE: ((DoWhile*)c->obj)->print();
        case IF: ((If*)c->obj)->print();
        case WHILE: ((While*)c->obj)->print();
        case FOR: ((For*)c->obj)->print();
        case PRINTF: ((Printf*)c->obj)->print();
        case SCANF: ((Scanf*)c->obj)->print();
        case EXIT: ((Exit*)c->obj)->print();
        case RETURN: ((Return*)c->obj)->print();


    }
}

void Program::print(){
    
    cout<<".data"<<"\t\t#rotulos e textos printf"<<endl;
    //todo printar os printf
    cout<<printF.size()<<endl;
    for(int i = 0 ; i < printF.size() ; i++){
        Printf  *pf = (Printf*)printF.at(i)->obj;
        pf->printLabel();
    }
    //todo printar os vetores
    cout<<".size"<<"\trotulos de vetores "<<endl;
    //printar a funcao;
    cout<<".text\t#codigo"<<endl;
    for(int i = functions.size()-1; i >= 0 ; i--){
        Function *f = (Function*)functions.at(i)->obj;
        f->print();
    }



}
//============== FUNCTION ====================

void Function::print(){
    rData = new RegisterData();
    cout<<id<<":"<<endl;
    evalSymbTable();
    printParam();
    for(int i = 0 ; i < commands.size() ; i++ ){
        container *c = commands.at(i);
        descricao(c->type);
        printObj(c);
    }
    cout<<"\n\n";

}

void Function::evalSymbTable(){
    map<string,container*>::iterator it;
    for( it = symbTable.begin() ; it != symbTable.end() ; it++){
        // cout<<"nome: "<<it->first<<endl;
        rData->addReg(it->first);
        
    }
}

void Function::printParam(){
    if(param.size()==0)
        return;
    for(int i = 0 ; i < param.size() ; i++){
        string id = ((Parameter*)param.at(i)->obj)->id;
        string reg = rData->addReg(id);
        cout<<"\tadd $"<<reg<<", $v"+to_string(i)<<", $zero"<<endl;
        
    }
}
//============== OPERATION ===================
OpLeaf* Operation::evalArithmeticLeaf(container *c){
    OpLeaf *leaf = (OpLeaf*)c->obj;
    Operation *op;
    int r = -1;
    if(c->type==OPLEAF){
        leaf = (OpLeaf*)c->obj;
        if(leaf->type==CONSTANT){
            r= rData->getNextRegister();
            cout<<"\taddi $t"<<r<<", $zero, "<<leaf->valueId<<std::endl;
            leaf->regTemp = r;
            leaf->type = OP_TEMPORARY;
            leaf->valueType = INT;
            leaf->valueId = "t"+to_string(r);
        }
    }else{
        op = (Operation*)c->obj;
        leaf = op->print();
    }
    return leaf;
}
OpLeaf* Operation::evalLogicalLeaf(container *c,string labelTrue,string labelFalse,bool demorgan){
    OpLeaf *leaf;
    Operation *op;
    int r = -1;
    if(c->type==OPLEAF){
        leaf = (OpLeaf*)c->obj;
        if(leaf->type==CONSTANT){
            r= rData->getNextRegister();
            cout<<"\taddi $t"<<r<<", $zero, "<<leaf->valueId<<std::endl;
            leaf->regTemp = r;
            leaf->type = OP_TEMPORARY;
            leaf->valueType = INT;
            leaf->valueId = "t"+to_string(r);
        }         
    }else{
        op = (Operation*)c->obj;
        if(op->opGroup==ARITHMETICAL){
            leaf = op->print();
        }else{
            op->printLogicalOperation(labelTrue,labelFalse,demorgan);
        }
    }
    return leaf;
}
OpLeaf* Operation::print(){
    OpLeaf *leftLeaf,*rightLeaf;
    
    leftLeaf = evalArithmeticLeaf(left);
    rightLeaf = evalArithmeticLeaf(right);
    
    OpLeaf *op = new OpLeaf();
    int r = rData->getNextRegister();
    op->type = OP_TEMPORARY;
    op->valueType = INT;
    op->regTemp=r;
    op->valueId = "t"+to_string(r);

    string r1 = op->valueId;
    string r2 = leftLeaf->valueId;
    if(!leftLeaf->type == OP_TEMPORARY)
        r2 = rData->getReg(r2);
    string r3 = rightLeaf->valueId;
    if(!rightLeaf->type == OP_TEMPORARY)
        r3 = rData->getReg(r3);
    
    switch(opType){
        case OP_ADD:    cout<<"\tadd $"<<r1<<",$"<<r2<<",$"<<r3<<std::endl;   break;
        case OP_SUB:    cout<<"\tsub $"<<r1<<",$"<<r2<<",$"<<r3<<std::endl;   break;
        case OP_MUL:    cout<<"\tmul $"<<r1<<",$"<<r2<<",$"<<r3<<std::endl;   break;
        case OP_DIV:    cout<<"\tdiv $"<<r1<<",$"<<r2<<",$"<<r3<<std::endl;   break;
        case OP_ASSIGN: cout<<"\tadd $"<<r2<<",$"<<r3<<",$zero"<<std::endl;   
                        rData->clearRegister(op->regTemp);                  break;
        default:        cout<<"\toperacao aritmetica erro"<<endl;             break;
    }
    if(leftLeaf->type == OP_TEMPORARY)
        rData->clearRegister(leftLeaf->regTemp);
    if(rightLeaf->type == OP_TEMPORARY)
        rData->clearRegister(rightLeaf->regTemp);
    return op;
}
bool Operation::printLogicalOperation(string labelTrue,string labelFalse,bool demorgan){
    OpLeaf *leftLeaf,*rightLeaf;
    Operation *leftOp,*rightOp;

    if(right==NULL){
        leftLeaf = (OpLeaf*)left->obj;
        cout<<"bne $"<<leftLeaf->valueId<<",$zero"<<labelTrue<<endl;
        return false;
    }
    if(opType==LOGICAL_AND)
        demorgan=true;
    else if (opType == LOGICAL_OR)
        demorgan=false;

    

    leftLeaf = evalLogicalLeaf(left,labelTrue,labelFalse,demorgan);
    rightLeaf = evalLogicalLeaf(right,labelTrue,labelFalse,demorgan);
    int r = rData->getNextRegister();
    cout<<"if tipo: ";
    descricao(opType);
    switch(opType){
        case GREATER_THAN:
            if(!demorgan){
                cout<<"\tslt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                cout<<"\tbne $t"<<r<<" ,$zero, "<<labelTrue<<endl;
            }else{
                cout<<"\tslt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelFalse<<endl;
            }
        break;
        case LESS_THAN:
            if(!demorgan){
                cout<<"\tslt $t"<<r<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<endl;
                cout<<"\tbne $t"<<r<<"$zero, "<<labelTrue<<endl;
            }else{
                cout<<"\tslt $t"<<r<<" ,$"<<leftLeaf->valueId<<" ,$"<<rightLeaf->valueId<<endl;
                cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelFalse<<endl;
            }
        break;
        case EQUAL:
            if(!demorgan){
                cout<<"\tbeq $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelTrue<<endl;
            }else{
                cout<<"\tbne $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelFalse<<endl;
            }
        break;
        case NOT_EQUAL:
            if(!demorgan){
                cout<<"\tbne $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelTrue<<endl;
            }else{
                cout<<"\tbeq $"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<", "<<labelFalse<<endl;
            }
        break;
        case LESS_EQUAL:
            if(!demorgan){
                cout<<"\tslt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelTrue;
            }else{
                cout<<"\tslt $t"<<r<<" ,$"<<rightLeaf->valueId<<" ,$"<<leftLeaf->valueId<<endl;
                cout<<"\tbne $t"<<r<<" ,$zero, "<<labelFalse<<endl;
            }

        break;
        case GREATER_EQUAL:
            if(!demorgan){
                cout<<"\tslt $t"<<r<<" ,$"<<leftLeaf->valueId<<" ,$"<<rightLeaf->valueId<<endl;
                cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelTrue<<endl;
            }else{
                cout<<"\tslt $t"<<r<<",$"<<leftLeaf->valueId<<",$"<<rightLeaf->valueId<<endl;
                cout<<"\tbne $t"<<r<<"$zero, "<<labelFalse<<endl;
            }
        break;
    
    }
    if(leftLeaf->type == OP_TEMPORARY)
        rData->clearRegister(leftLeaf->regTemp);
    if(rightLeaf->type == OP_TEMPORARY)
        rData->clearRegister(rightLeaf->regTemp);
    rData->clearRegister(r);
    return demorgan;
}
//============== DO WHILE ====================
void DoWhile::print(){
    Operation *op = (Operation*)condition->obj;
    string lblTrue = "do_while";
    string lblFalse = "end_do_while";
    cout<<lblTrue<<":"<<endl;
    cout<<"\n\n...\n\n"<<endl;
    bool t = op->printLogicalOperation(lblTrue,lblFalse,false);
    // se não for AND 
    if(!t){
        cout<<"j "<<lblFalse<<endl;
    }else{
        cout<<"j "<<lblTrue<<endl;
    }
    cout<<lblFalse<<endl;
    
}
//================ IF ========================
void If::print(){
    container *ct = exp.front();
    Operation *op = (Operation*)ct->obj;
    bool t = op->printLogicalOperation("then","else",false);
    // se não for AND 
    if(!t){
        cout<<"\tj else"<<endl;
    }
    cout<<"then:"<<endl;
    cout<<"\tj fim"<<endl;
    cout<<"else:"<<endl;
    cout<<"fim:"<<endl;
    
}
//================ FOR =======================

void For::print(){
    Operation *opInit = (Operation*)init->obj;
    opInit->print();

    string lblTrue = "end_for";
    string lblFalse = "for";
    cout<<lblFalse<<":"<<endl;
    Operation *opCondition = (Operation*)condition->obj;
    opCondition->printLogicalOperation(lblTrue,lblFalse,false);
    cout<<"\n\n...\n\n"<<endl;

    Operation *adjust = (Operation*)adjustment->obj;
    adjust->print();
    cout<<"\tj for"<<endl;

}
//=============== WHILE ======================
void While::print(){
    Operation *op = (Operation*)condition->obj;
    string lblTrue = "end_while";
    string lblFalse = "while";
    cout<<lblFalse<<":"<<endl;
    bool t = op->printLogicalOperation(lblTrue,lblFalse,false);
    // se não for AND 
    cout<<"\n\n...\n\n"<<endl;
    if(!t){
        cout<<"\tj "<<lblFalse<<endl;
    }else{
        cout<<"\tj "<<lblTrue<<endl;
    }
    cout<<lblFalse<<endl;
    
}
//============== RETURN ======================
void Return::print(){
    return;
}
//=============== SCANF ======================
void Scanf::print(){
    cout<<"tipo do endereco: "<<address->type<<endl;
    cout<<"texto do scanf: "<<str<<endl;
    return;
}
//=============== Exit =======================
void Exit::print(){
    return;
}
//============== PRINTF ======================
void Printf::print(){
    cout<<"\tprintf: "<<str<<endl;
}

void Printf::printLabel(){
    str = "Entre com um valor inteiro:";

}





//================ REGISTER DATA =======================

string RegisterData::addReg(string nome){
    int r = getNextRegister();
    string reg = "t"+to_string(r);
    regName[nome] = reg;
    return reg;
}
std::string RegisterData::getReg(string nome){
    return regName[nome];
}

void RegisterData::clearRegister(int r){
    tempRegisters[r]=false;
}

int RegisterData::getNextRegister(){
    int r;
    for(r = 0; r<10;r++){
        if(!tempRegisters[r]){
            tempRegisters[r]=true;
            return r;
        }        
    }
    return 9;
}
