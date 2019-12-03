#include <iostream>
#include <string>
#include <vector>
#include "classes.h"
using namespace std;

RegisterData *rData;
bool firstFunction;

void descricao(int d){
    switch(d){
        case PROGRAM: cout<<"\t#PROGRAM"<<endl;        return;
        case CONSTANT: cout<<"\t#CONSTANT"<<endl;              return;
        case GLOBALVARIABLE: cout<<"\t#GLOBALVARIABLE"<<endl;  return;
        case FUNCTION: cout<<"\t#FUNCTION"<<endl;              return;
        case PARAMETER: cout<<"\t#PARAMETER"<<endl;            return;
        case VARIABLE: cout<<"\t#VARIABLE"<<endl;              return;
        case DOWHILE: cout<<"\t#DOWHILE"<<endl;                return;
        case IF: cout<<"\t#IF"<<endl;                          return;
        case WHILE: cout<<"\t#WHILE"<<endl;                    return;
        case FOR: cout<<"\t#FOR"<<endl;                        return;
        case PRINTF: cout<<"\t#PRINTF"<<endl;                  return;
        case SCANF: cout<<"\t#SCANF"<<endl;                    return;
        case EXIT: cout<<"\t#EXIT"<<endl;                      return;
        case RETURN: cout<<"\t#RETURN"<<endl;                  return;
        case OPERATION: cout<<"\t#OPERATION"<<endl;            return;
        case OPRESULT: cout<<"\t#OPRESULT"<<endl;              return;
        case OPLEAF: cout<<"\t#OPLEAF"<<endl;                  return;
        case OP_ADD: cout<<"\t#OP_ADD"<<endl;                  return;
        case OP_SUB: cout<<"\t#OP_SUB"<<endl;                  return;
        case OP_MUL: cout<<"\t#OP_MUL"<<endl;                  return;
        case OP_DIV: cout<<"\t#OP_DIV"<<endl;                  return;
        case OP_VARIABLE: cout<<"\t#OP_VARIABLE"<<endl;        return;
        case OP_CONSTANT: cout<<"\t#OP_CONSTANT"<<endl;        return;
        case OP_FUNCTION: cout<<"\t#OP_FUNCTION"<<endl;        return;
        case OP_ASSIGN: cout<<"\t#OP_ASSIGN"<<endl;            return;
        case INT: cout<<"\t#INT"<<endl;                        return;
        case CHAR: cout<<"\t#CHAR"<<endl;                      return;
        case LOGICAL_AND: cout<<"\t#LOGICAL_AND"<<endl;        return;
        case LOGICAL_OR: cout<<"\t#LOGICAL_OR"<<endl;          return;
        case GREATER_THAN: cout<<"\t#GREATER_THAN"<<endl;      return;
        case LESS_THAN: cout<<"\t#LESS_THAN"<<endl;            return;
        case EQUAL: cout<<"\t#EQUAL"<<endl;                    return;
        case NOT_EQUAL: cout<<"\t#NOT_EQUAL"<<endl;            return;
        case LESS_EQUAL: cout<<"\t#LESS_EQUAL"<<endl;          return;
        case GREATER_EQUAL: cout<<"\t#GREATER_EQUAL"<<endl;    return;
        case ARITHMETICAL: cout<<"\t#ARITHMETICAL"<<endl;      return;
        case LOGICAL: cout<<"\t#LOGICAL"<<endl;                return;
        case OP_TEMPORARY: cout<<"\t#OP_TEMPORARY"<<endl;      return;
    }  
}

void printObj(container *c){
    switch(c->type){

        case OPERATION: ((Operation*)c->obj)->print();  return;
        case DOWHILE: ((DoWhile*)c->obj)->print();      return;
        case IF: ((If*)c->obj)->print();                return;
        case WHILE: ((While*)c->obj)->print();          return;
        case FOR: ((For*)c->obj)->print();              return;
        case PRINTF: ((Printf*)c->obj)->print();        return;
        case SCANF: ((Scanf*)c->obj)->print();          return;
        case EXIT: ((Exit*)c->obj)->print();            return;
        case RETURN: ((Return*)c->obj)->print();        return;


    }
}

void Program::print(){
    
    printPrintf();
    //todo printar os vetores
    // cout<<".size"<<"\trotulos de vetores "<<endl;
    //printar a funcao;
    cout<<".text\t#codigo"<<endl;
    firstFunction = true;
    for(int i = functions.size()-1; i >= 0 ; i--){
        Function *f = (Function*)functions.at(i)->obj;
        f->print();
        firstFunction = false;
    }



}
void Program::printPrintf(){
    if(printF.size()==0)
        return;
    cout<<".data"<<"\t\t#rotulos e textos printf"<<endl;
    for(int i = 0 ; i < printF.size() ; i++){
        Printf  *pf = (Printf*)printF.at(i)->obj;
        pf->printLabel();
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
        if(it->second != 0){
            cout<<"#"<<it->first;
            rData->addReg(it->first);
        }
        
    }
}

void Function::printParam(){
    if(param.size()==0)
        return;
    for(int i = 0 ; i < param.size() ; i++){
        container *c = param.at(i);
        string id = ((Parameter*)param.at(i)->obj)->id;
        descricao(c->type);
        string reg = rData->getReg(id);
        cout<<"\tadd $"<<reg<<", $a"+to_string(i)<<", $zero"<<endl;
        
    }
}
//============== OPERATION ===================
OpLeaf* Operation::evalArithmeticLeaf(container *c){
    OpLeaf *leaf = (OpLeaf*)c->obj;
    Operation *op;
    int r = -1;
    if(c->type==OPLEAF){
        leaf = (OpLeaf*)c->obj;
        if(leaf->type==OP_CONSTANT){
            r= rData->getNextRegister();
            cout<<"\taddi $t"<<r<<", $zero, "<<leaf->valueId<<std::endl;
            leaf->regTemp = r;
            leaf->type = OP_TEMPORARY;
            leaf->valueType = INT;
            leaf->valueId = to_string(r);
        }else if(leaf->type == OP_FUNCTION){
            leaf = leaf->printFunction();
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
        if(leaf->type==OP_CONSTANT){
            r= rData->getNextRegister();
            cout<<"\taddi $t"<<r<<", $zero, "<<leaf->valueId<<std::endl;
            leaf->regTemp = r;
            leaf->type = OP_TEMPORARY;
            leaf->valueType = INT;
            leaf->valueId = to_string(r);
        }         
    }else{
        op = (Operation*)c->obj;
        
        if(op->verifyGroup()==ARITHMETICAL){
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
    op->valueId = to_string(r);

    string r1 = op->valueId;
    string r2 = leftLeaf->valueId;
    if(leftLeaf->type != OP_TEMPORARY)
        r2 = rData->getReg(r2);
    else
        r2 = "t" + r2;
    string r3 = rightLeaf->valueId;
    if(rightLeaf->type != OP_TEMPORARY)
        r3 = rData->getReg(r3);
    else
        r3 = "t" + r3;
    switch(opType){
        case OP_ADD:    std::cout<<"\tadd $t"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
        case OP_SUB:    std::cout<<"\tsub $t"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
        case OP_MUL:    std::cout<<"\tmul $t"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
        case OP_DIV:    std::cout<<"\tdiv $t"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
        case OP_ASSIGN: std::cout<<"\tadd $"<<r2<<", $"<<r3<<", $zero"<<std::endl;   
                        rData->clearRegister(op->regTemp);                           break;
        default:        std::cout<<"\toperacao aritmetica erro"<<std::endl;          break;
    }
    if(leftLeaf->type == OP_TEMPORARY)
        rData->clearRegister(leftLeaf->regTemp);
    if(rightLeaf->type == OP_TEMPORARY)
        rData->clearRegister(rightLeaf->regTemp);
    return op;
}
bool Operation::printLogicalOperation(string labelTrue,string labelFalse,bool demorgan){
    descricao(opType);
    OpLeaf *leftLeaf,*rightLeaf;
    Operation *leftOp,*rightOp;

    if(right==NULL){
        leftLeaf = (OpLeaf*)left->obj;
        std::cout<<"bne $"<<leftLeaf->valueId<<",$zero"<<labelTrue<<std::endl;
        return false;
    }
    if(opType==LOGICAL_AND)
        demorgan=true;
    else if (opType == LOGICAL_OR)
        demorgan=false;


    leftLeaf = evalLogicalLeaf(left,labelTrue,labelFalse,demorgan);
    rightLeaf = evalLogicalLeaf(right,labelTrue,labelFalse,demorgan);
    int r = rData->getNextRegister();
    // cout<<"voltou: ";
    // descricao(opType);
    if(opType==LOGICAL_AND || opType == LOGICAL_OR){
        return demorgan;
    }
    string r1 = leftLeaf->valueId;
    cout<<"#AMIGO ESTOUAQUI:";
    descricao(rightLeaf->type);
    if(leftLeaf->type !=OP_TEMPORARY)
        r1 = rData->getReg(leftLeaf->valueId);
      else
        r1 = "t" + r1;
    string r2 = rightLeaf->valueId;
    if(rightLeaf->type != OP_TEMPORARY)
        r2= rData->getReg(rightLeaf->valueId);
      else
        r2 = "t" + r2;

    switch(opType){
        case GREATER_THAN:
            if(!demorgan){
                std::cout<<"\tslt $t"<<r<<" ,$"<<r2<<" ,$"<<r1<<std::endl;
                std::cout<<"\tbne $t"<<r<<" ,$zero, "<<labelTrue<<std::endl;
            }else{
                std::cout<<"\tslt $t"<<r<<" ,$"<<r2<<" ,$"<<r1<<std::endl;
                std::cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelFalse<<std::endl;
            }
        break;
        case LESS_THAN:
            if(!demorgan){
                std::cout<<"\tslt $t"<<r<<",$"<<r1<<",$"<<r2<<std::endl;
                std::cout<<"\tbne $t"<<r<<"$zero, "<<labelTrue<<std::endl;
            }else{
                std::cout<<"\tslt $t"<<r<<" ,$"<<r1<<" ,$"<<r2<<std::endl;
                std::cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelFalse<<std::endl;
            }
        break;
        case EQUAL:
            if(!demorgan){
                std::cout<<"\tbeq $"<<r1<<",$"<<r2<<", "<<labelTrue<<std::endl;
            }else{
                std::cout<<"\tbne $"<<r1<<",$"<<r2<<", "<<labelFalse<<std::endl;
            }
        break;
        case NOT_EQUAL:
            if(!demorgan){
                std::cout<<"\tbne $"<<r1<<",$"<<r2<<", "<<labelTrue<<std::endl;
            }else{
                std::cout<<"\tbeq $"<<r1<<",$"<<r2<<", "<<labelFalse<<std::endl;
            }
        break;
        case LESS_EQUAL:
            if(!demorgan){
                std::cout<<"\tslt $t"<<r<<" ,$"<<r2<<" ,$"<<r1<<std::endl;
                std::cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelTrue<<std::endl;
            }else{
                std::cout<<"\tslt $t"<<r<<" ,$"<<r2<<" ,$"<<r1<<std::endl;
                std::cout<<"\tbne $t"<<r<<" ,$zero, "<<labelFalse<<std::endl;
            }

        break;
        case GREATER_EQUAL:
            if(!demorgan){
                std::cout<<"\tslt $t"<<r<<" ,$"<<leftLeaf->valueId<<" ,$"<<r2<<std::endl;
                std::cout<<"\tbeq $t"<<r<<" ,$zero, "<<labelTrue<<std::endl;
            }else{
                std::cout<<"\tslt $t"<<r<<",$"<<leftLeaf->valueId<<",$"<<r2<<std::endl;
                std::cout<<"\tbne $t"<<r<<"$zero, "<<labelFalse<<std::endl;
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
int Operation::verifyGroup(){
    switch(opType){
        case OP_ADD:    return ARITHMETICAL;
        case OP_SUB:    return ARITHMETICAL;
        case OP_MUL:    return ARITHMETICAL;
        case OP_DIV:    return ARITHMETICAL;
        case OP_ASSIGN: return ARITHMETICAL;
    }
    return LOGICAL;
}

//================ OpLeaf ========================
OpLeaf* OpLeaf::printFunction(){
    OpLeaf * op =  new OpLeaf();
    rData->stackUp();
    cout<<"\t#PARAMETERS"<<endl;
    printParameters();
    op->type = OP_TEMPORARY;
    cout<<"\tjal "<<valueId<<endl;
    rData->unstack();
    op->regTemp = rData->getNextRegister();
    cout<<"\tadd $t"<<op->regTemp<<", $v0, $zero"<<endl;
    op->valueId = to_string(op->regTemp);
    return op;
}
void OpLeaf::printParameters(){
    OpLeaf *opl;
    Operation *op;
    for(int i = 0 ; i < values.size(); i++){
        container *c = values.at(i);
        if(c->type == OPLEAF){
            opl = (OpLeaf*) c->obj;
            if(opl->type == OP_CONSTANT){
            cout<<"\taddi $a"<<i<<", $zero"<<", $"<<opl->valueId<<endl;
            }else if(opl->type == OP_VARIABLE){
                cout<<"\tadd $a"<<i<<", $"<<rData->getReg(opl->valueId)<<", $zero"<<endl;
            }

        }else{
            op = (Operation*)c->obj;
            opl = (OpLeaf*)op->print();
            
            cout<<"\tadd $a"<<i<<", $t"<<opl->valueId<<", $zero"<<endl;
            rData->clearRegister(opl->regTemp);
        }
    }
}

//============== DO WHILE ====================
void DoWhile::print(){
    Operation *op = (Operation*)condition->obj;
    string idlb = to_string(idLabel);
    string lblTrue = "do_while"+idlb;
    string lblFalse = "end_do_while"+idlb;
    cout<<lblTrue<<":"<<endl;
    // todo printar comandos
    cout<<"\n\n...\n\n"<<endl;
    bool t = op->printLogicalOperation(lblTrue,lblFalse,false);
    // se não for AND 
    if(!t){
        cout<<"j "<<lblFalse<<endl;
    }else{
        cout<<"j "<<lblTrue<<endl;
    }
    cout<<lblFalse<<":"<<endl;
    
}
//================ IF ========================
void If::print(){
    container *ct = exp.front();
    Operation *op = (Operation*)ct->obj;
    string idlb = to_string(idLabel);
    string labelTrue = "then"+idlb;
    string labelFalse = "else"+idlb;
    string labelEnd = "fimIf"+idlb;
    bool t = op->printLogicalOperation(labelTrue,labelFalse,false);
    // se não for AND 
    if(!t){
        cout<<"\tj "<<labelFalse<<endl;
    }
    cout<<labelTrue<<":"<<endl;
    cout<<"\t#COMANDOS - THEN - IF"<<idlb<<endl;
    for(int i = 0 ; i < then.size() ; i++ ){
        container *c = then.at(i);
        descricao(c->type);
        printObj(c);
    }
    cout<<"\tj "<<labelEnd<<endl;
    cout<<labelFalse<<":"<<endl;
    if(els.size()>0){
        cout<<"\t#COMANDOS - ELSE - IF"<<idlb<<endl;
        for(int i = 0 ; i < els.size() ; i++ ){
            container *c = els.at(i);
            descricao(c->type);
            printObj(c);
        }
    }
    cout<<"\tj "<<labelEnd<<endl;
    
    cout<<labelEnd<<":"<<endl;
    
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
    Operation *op ;
    OpLeaf *opl;
    if(firstFunction){
        cout<<"addi $v0, $zero, 10"<<endl;
        cout<<"syscall"<<endl;
        return;
    }
    if(exp->type == OPERATION){
        op = (Operation*)exp->obj;
        opl = (OpLeaf*)op->print();
        cout<<"\tadd $v0, $t"<<opl->valueId<<", $zero"<<endl;
        rData->clearRegister(opl->regTemp);
    }else if( exp->type == OPLEAF){
        opl = (OpLeaf*) exp->obj;
        if(opl->type == OP_CONSTANT){
            cout<<"\taddi $v0, $zero"<<", "<<opl->valueId<<endl;
        }else if(opl->type == VARIABLE){
            cout<<"\tadd $v0, $"<<rData->getReg(opl->valueId)<<", $zero"<<endl;
        }

    }
    cout<<"\tjr $ra"<<endl;
    return;
}
//=============== SCANF ======================
void Scanf::print(){
     if(str.find("%d") != string::npos){
         cout<<"\taddi $v0, $zero,5"<<endl;;
         cout<<"\tsyscall"<<endl;
     }else if(str.find("%c") != string::npos) {
        cout<<"\taddi $v0, $zero,12"<<endl;;
        cout<<"\tsyscall"<<endl;
     }
    OpLeaf *opl = ((OpLeaf*)((Operation*)address->obj)->left->obj);
    cout<<"\tadd $"<<rData->getReg(opl->valueId)<<", $v0 ,$zero"<<endl;
        return;
}
//=============== Exit =======================
void Exit::print(){
    return;
}
//============== PRINTF ======================
void Printf::print(){
    int qtdPar = 0;
    for(int i = 0 ; i < dataLabels.size(); i++){
        string label = dataLabels.at(i);
        if(label!=""){
            cout<<"\taddi $v0, $zero, 4"<<endl;
            cout<<"\tla $a0, "<<label<<endl;
            cout<<"\tsyscall"<<endl;
        }else{
            container* c = exp.at(qtdPar);
            OpLeaf *opl;
            if(c->type==OPLEAF){
                opl = (OpLeaf*)c->obj;
                if(opl->type == OP_VARIABLE){
                    cout<<"\taddi $v0, $zero, 1"<<endl;
                    cout<<"\tadd $a0, $zero, $"<<rData->getReg(opl->valueId)<<endl;
                    cout<<"\tsyscall"<<endl;
                }else if (opl->type == OP_FUNCTION){
                    opl = opl->printFunction();
                    cout<<"\taddi $v0, $zero, 1"<<endl;
                    if(opl->type == OP_TEMPORARY){
                        cout<<"\tadd $a0, $zero, $t"<<opl->valueId<<endl;
                        rData->clearRegister(opl->regTemp);
                    }else{
                        cout<<opl->valueId<<endl;
                        cout<<"\tadd $a0, $zero, $"<<rData->getReg(opl->valueId)<<endl;
                    }
                    cout<<"\tsyscall"<<endl;
                    rData->clearRegister(opl->regTemp);
                }
            }
            qtdPar++;
        }

    }
}
void Printf::printLabel(){
    int pos = 0;
    string label = "printF"+to_string(idLabel);
    // se a string for formatada:
    if((pos = str.find("%d")) != string::npos){
        string subst= str.substr(0,str.length()-1);//copia a string
        subst = subst.erase(0,1);//apaga aspas no inicio
        subst = subst.erase(subst.length());//apaga aspas no fim
        int c = 0;
        pos-=1;
        while(true){
            string s = subst.substr(0,pos);
            // cout<<"\""<<s<<"\""<<endl;
            // for(int i = 0 ; i < s.length();i++)cout<<" ";
            // cout<<"^"<<endl;
            cout<<"\t"<<label<<c<<": .asciiz \""<<s<<"\""<<endl;
            subst.erase(0,pos+2);
            dataLabels.push_back(label+to_string(c));
            dataLabels.push_back("");
            c++;
            if((pos = subst.find("%d")) == string::npos){
                    cout<<"\t"<<label<<c<<": .asciiz \""<<subst<<"\"\n"<<endl;
                    dataLabels.push_back(label+to_string(c));
                break;
            }
        }
    }else{
        cout<<"\t"<<label<<":  .asciiz "<<str<<endl;
        dataLabels.push_back(label);
    }

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
    // cout<<"#desalocou registrador"<<endl;
    tempRegisters[r]=false;
}

int RegisterData::getNextRegister(){
    int r;
    // cout<<"#alocou registrador";
    for(r = 0; r<10;r++){
        if(!tempRegisters[r]){
            tempRegisters[r]=true;
            // cout<<r<<endl;
            return r;
        }        
    }
    return 9;
}
void RegisterData::stackUp(){
    cout<<"\t#EMPILHAR"<<endl;
    for(int i = 0; i < 10 ; i++){
        if(tempRegisters[i]){
            cout<<"\taddiu $sp, $sp, -4"<<endl;
            cout<<"\tsw $t"<<i<<", 0($sp)"<<endl;
        }
    }
    cout<<"\taddiu $sp, $sp, -4"<<endl;
    cout<<"\tsw $ra, 0($sp)"<<endl;
    
}
void RegisterData::unstack(){
    cout<<"\t#DESEMPILHAR"<<endl;
    cout<<"\tlw $ra, 0($sp)"<<endl;
    cout<<"\taddiu $sp, $sp, 4"<<endl;
    for(int i = 10; i >= 0 ; i--){
        if(tempRegisters[i]){
            cout<<"\tlw $t"<<i<<", 0($sp)"<<endl;
            cout<<"\taddiu $sp, $sp, 4"<<endl;
        }
    }
}