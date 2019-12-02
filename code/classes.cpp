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
    cout<<".size"<<"\trotulos de vetores "<<endl;
    //printar a funcao;
    cout<<".text\t#codigo"<<endl;
    for(int i = functions.size()-1; i >= 0 ; i--){
        Function *f = (Function*)functions.at(i)->obj;
        f->print();
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
        case OP_ADD:    std::cout<<"\tadd $"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
        case OP_SUB:    std::cout<<"\tsub $"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
        case OP_MUL:    std::cout<<"\tmul $"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
        case OP_DIV:    std::cout<<"\tdiv $"<<r1<<", $"<<r2<<", $"<<r3<<std::endl;   break;
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
    descricao(opType);
    string r1 = rData->getReg(leftLeaf->valueId);
    string r2 = rData->getReg(rightLeaf->valueId);
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
    // todo printar comandos
    cout<<"\n\n...\n\n"<<endl;
    cout<<"\tj "<<labelEnd<<endl;
    cout<<labelFalse<<":"<<endl;
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
    return;
}
//=============== SCANF ======================
void Scanf::print(){

    cout<<"\ttexto do scanf: "<<str<<endl;
    return;
}
//=============== Exit =======================
void Exit::print(){
    return;
}
//============== PRINTF ======================
void Printf::print(){
    cout<<"#PRINTF"<<endl;
    int qtdPar = 0;
    for(int i = 0 ; i < dataLabels.size(); i++){
        string label = dataLabels.at(i);
        if(label!=""){
            cout<<"\tli $v0, 4"<<endl;
            cout<<"\tla $a0, "<<label<<endl;
            cout<<"\tsyscall"<<endl;
        }else{
            container* c = exp.at(qtdPar);
            OpLeaf *opl;
            if(c->type==OPLEAF){
                opl = (OpLeaf*)c->obj;
                if(opl->type == OP_VARIABLE){
                    cout<<"\tli $v0, 1"<<endl;
                    cout<<"\tadd $a0, $zero, $"<<rData->getReg(opl->valueId)<<endl;
                    cout<<"\tsyscall"<<endl;
                }else if (opl->type == OP_FUNCTION){
                    
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
