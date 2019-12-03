#include <iostream>
#include <string>
#include <vector>
#include "classes.h"

using namespace std;



std::string readInput();
std::string getObjType(std::string line);
int getNum(std::string str);
std::vector<std::string> split(std::string str);

void readProgram();
Constant* readConstant(std::string arg);
GlobalVariable* readGlobalVariable(std::string arg);
Function* readFunction();
While* readWhile(std::string str);
DoWhile* readDoWhile(std::string str);
For* readFor(std::string str);
Parameter* readParameter();
If* readIf(std::string str);
Operation* readOperation(std::string arg);
OpLeaf* readOpLeaf(std::string arg);
Return* readReturn(std::string arg);
Variable* readVariable();
Printf* readPrintf(std::string arg);
Scanf* readScanf(std::string arg);

int FILEEND = 0, contPrintf = 0, contIf = 0, contDoWhile = 0, contWhile = 0, contFor = 0;
std::string LINE;
Program *AST;
Function *CURRENTFUNCTION;


std::string readInput() {
    char c;
    std::string str = "";
    int n = 0;

    while(scanf("%c", &c)!=EOF){
        if(c == '\n'){n = 1; break;}
        if(str == "" && c == ' '){
            continue;
        }else{
            str.append(1, c);
        }
    }

    if(!n){FILEEND = 1;}

    return str;
}

std::string getObjType(std::string line) {
    int i = 0;
    char c = ' ';
    std::string str = "";
    c = line.at(0);
    while(!(c >= 65 && c <= 90) && !(c >= 97 && c <= 122) && !(c >= '0' && c <= '9') &&  c != '_' &&
            c != '+' && c != '-' && c != '*' && c != '/' && c != '%' && c != '&' && c != '|' && 
            c != '^' && c != '=' && c != '!' && c != '<' && c != '>' && c != '-' && c != '~') {
        i++;
        if(i >= line.size()) {
            break;
        }
        c = line.at(i);
    }
    c = line.at(i);
    while((c >= 65 && c <= 90) || (c >= 97 && c <= 122) && !(c >= '0' && c <= '9') ||  c == '_' ||
            c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '&' || c == '|' || 
            c == '^' || c == '=' || c == '!' || c == '<' || c == '>' || c == '-' || c == '~'){
        str.append(1, c);
        i++;
        if(i >= line.size()) {
            break;
        }
        c = line.at(i);
    }  
    return str;
};

int getNum(std::string str) {
    if(str == "AST") return PROGRAM;
    if(str == "CONSTANT") return CONSTANT;
    if(str == "GLOBAL") return GLOBALVARIABLE;
    if(str == "FUNCTION") return FUNCTION;
    if(str == "PARAMETER") return PARAMETER;
    if(str == "VARIABLE") return VARIABLE;
    if(str == "DO_WHILE") return DOWHILE;
    if(str == "IF") return IF;
    if(str == "WHILE") return WHILE;
    if(str == "FOR") return FOR;
    if(str == "PRINTF") return PRINTF;
    if(str == "SCANF") return SCANF;
    if(str == "EXIT") return EXIT;
    if(str == "RETURN") return RETURN;
    if(str == "+") return OPERATION;
    if(str == "-") return OPERATION;
    if(str == "*") return OPERATION;
    if(str == "/") return OPERATION;
    if(str == "=") return OPERATION;
    if(str == "&&") return OPERATION;
    if(str == "||") return OPERATION;
    if(str == ">") return OPERATION;
    if(str == "<") return OPERATION;
    if(str == "==") return OPERATION;
    if(str == "!=") return OPERATION;
    if(str == "<=") return OPERATION;
    if(str == ">=") return OPERATION;
    if(str == "&") return OPERATION;
  //if(str == "") return ;

    return 999;
}

int getOpType(std::string str) {
    if(str == "+") return OP_ADD;
    if(str == "-") return OP_SUB;
    if(str == "*") return OP_MUL;
    if(str == "/") return OP_DIV;
    if(str == "=") return OP_ASSIGN;
    if(str == "&&") return LOGICAL_AND;
    if(str == "||") return LOGICAL_OR;
    if(str == ">") return GREATER_THAN;
    if(str == "<") return LESS_THAN;
    if(str == "==") return EQUAL;
    if(str == "!=") return NOT_EQUAL;
    if(str == "<=") return LESS_EQUAL;
    if(str == ">=") return GREATER_EQUAL;
    if(str == "&") return REF;
  //if(str == "") return ;
    return 999;
}

int getOpGroup(std::string str) {
    if(str == "+") return ARITHMETICAL;
    if(str == "-") return ARITHMETICAL;
    if(str == "*") return ARITHMETICAL;
    if(str == "/") return ARITHMETICAL;
    if(str == "=") return ARITHMETICAL;
    if(str == "&") return ARITHMETICAL;
    if(str == "&&") return LOGICAL;
    if(str == "||") return LOGICAL;
    if(str == ">") return LOGICAL;
    if(str == "<") return LOGICAL;
    if(str == "==") return LOGICAL;
    if(str == "!=") return LOGICAL;
    if(str == "<=") return LOGICAL;
    if(str == ">=") return LOGICAL;
  //if(str == "") return ;
    return 999;
}


bool isNumber(std::string str) {
    if(str.at(0) >= 48 && str.at(0) <= 57) {
        return true;
    }
    return false;
};

std::vector<std::string> split(std::string str) {
    std::vector<std::string> tokens;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    token = str.substr(0, pos);
    tokens.push_back(token);
    return tokens;
}

//retorna tokens separadas por virgula
std::vector<std::string> splitComma(std::string str) {
    std::vector<std::string> tokens;
    std:: string s = "";
    char c;
    int i = 0, flag = 0;;
    

    while(str.at(i) != '('){
        i++;
    }
    i++;
    while(true) {
        c = str.at(i);
        if(c == ')') {
            tokens.push_back(s);
            break;
        }
        if(c == ','){ //se encontra virgula adiciona a expressao no vetor
            tokens.push_back(s);
            s = "";
        }else if(c == '(') { //se encontra a abertura de parenteses continua copiando ate encontrar o fechamento
            flag++;
            while(flag > 0) {
                s.append(1, c);
                i++;
                c = str.at(i);
                if(c == ')') {
                    flag--;
                }else if(c == '(') {
                    flag++;
                }
            }
            s.append(1, c);
        }else if(i >= str.size()-1){
            tokens.push_back(s);
            break;
        }else{ //caso encontre um char qualquer, continua adicionaod aa string
            s.append(1, c);
        }
        i++;
    }

    
    return tokens;

}

//separa tokens por ponto-e-virgula
std::vector<std::string> splitSemiCollon(std::string str) {
    std::string s = "";
    std::vector<std::string> tokens;

    char c;
    int i = 0, flag = 0;

    while(i < str.size()) {
        c = str.at(i);
        if(c == ';'){
            tokens.push_back(s);
            s = "";
        } else if(c == '(') { //se encontra a abertura de parenteses continua copiando ate encontrar o fechamento
            flag++;
            while(flag > 0) {
                s.append(1, c);
                i++;
                c = str.at(i);
                if(c == ')') {
                    flag--;
                }else if(c == '(') {
                    flag++;
                }
            }
            s.append(1, c);
        } else {
            s.append(1, c);
        }
        i++;
    }
    if(s.size() > 0){
        tokens.push_back(s);
    }

    return tokens;
}

std:: vector<std::string> splitOperation(std::string str) {
    std::string s = "";
    std::vector<std::string> tokens;
    int flag = 0;
    char c;
    int i = 0;
    c = str.at(i);
    while(c != '(') {
        s.append(1, c);
        i++;
        if(i >= str.size()){
            tokens.push_back(s);
            return tokens;
        }
        c = str.at(i);
    }
    tokens.push_back(s);
    s = "";
    i++;
    c = str.at(i);
    while(c != ')') {
        if(c == '(') {
            flag ++;
            while(flag > 0) {
                s.append(1, c);
                i++;
                c = str.at(i);
                if(c == '(') {
                    flag++;
                }else if(c == ')'){
                    flag--;
                }
            }
            s.append(1, c);
        } else if(c == ',') {
            tokens.push_back(s);
            s = "";
        } else {
            s.append(1, c);
        }
        i++;
        c = str.at(i);
    }
    if(s.size() > 0){
        tokens.push_back(s);
    }

    return tokens;
}

void readProgram() {
    cout << "read program\n";
    container *cont;
    Constant *c;
    GlobalVariable *gv;
    Function *f;

    AST = new Program();
    CURRENTFUNCTION = f;

    LINE = readInput();
    while(FILEEND != 1) {
        if(LINE != "" && getNum(getObjType(LINE)) != PROGRAM){
            switch(getNum(getObjType(LINE))) {
                case FUNCTION:
                    f = readFunction();
                    cont = new container();
                    cont->type = FUNCTION;
                    cont->obj = f;
                    AST->functions.push_back(cont);
                    break;
                case CONSTANT:
                    c = readConstant(LINE);
                    cont = new container();
                    cont->type = CONSTANT;
                    cont->obj = f;
                    AST->symbTable[c->id] = cont;
                    break;
                case GLOBALVARIABLE:
                    gv = readGlobalVariable(LINE);
                    cont = new container();
                    cont->type = GLOBALVARIABLE;
                    cont->obj = gv;
                    AST->symbTable[gv->id] = cont;
                    break;
                default:
                    break;
            }
        }
        LINE = readInput();
    }

    //read consant
    //read global variable
    //read functions    
}


Constant* readConstant(std::string arg) {
    cout << "read constant\n";
    Constant *cnst = new Constant();    
    std::vector<std::string> splitLine = split(arg);
    cnst->id = splitLine[1];
    cnst->value = splitLine[3];
    return cnst;
}

GlobalVariable* readGlobalVariable(std::string arg) {
    cout << "read global variable\n";
    GlobalVariable *gv = new GlobalVariable();
    std::vector<std::string> splitLine = split(arg);
    gv->id = splitLine[2];
    gv->type = splitLine[4];
    return gv;
}


While* readWhile(std::string str) {
    cout << "read while\n";
    While *w = new While(), *w2;
    DoWhile *dw;
    Operation *op;
    OpLeaf *opl;
    container *c;
    Return *r;
    If *iff;
    Printf *prt;
    Scanf *scf;
    For* fr;
    int i;
    std::vector<std::string> arg, commands;


    w->idLabel = contWhile;
    contWhile++;
    arg = splitComma(str);
    //conditions
    //command list
    if(getNum(getObjType(arg[0])) == OPERATION) {//operacao
        op = readOperation(arg[0]);
        c = new container();
        c->type = OPERATION;
        c->obj = op;
        w->condition = c;
    }else{//opleaf
        opl = readOpLeaf(arg[0]);
        c = new container();
        c->type = OPLEAF;
        c->obj = opl;
        w->condition = c;
    }
    commands = splitSemiCollon(arg[1]);
    for(i = 0; i < commands.size(); i++) {
        switch(getNum(getObjType(commands[i]))){
            case RETURN:
                r = readReturn(commands[i]);
                c = new container();
                c->type = RETURN;
                c->obj = r;
                w->commands.push_back(c);
                break;
            case OPERATION:
                op = readOperation(commands[i]);
                c = new container();
                c->type = OPERATION;
                c->obj = op;
                w->commands.push_back(c);
                break;
            case IF:
                iff = readIf(commands[i]);
                c = new container();
                c->type = IF;
                c->obj = iff;
                w->commands.push_back(c);
                break;
            case WHILE:
                w2 = readWhile(commands[i]);
                c = new container();
                c->type = WHILE;
                c->obj = w2;
                w->commands.push_back(c);
                break;
            case DOWHILE:
                dw = readDoWhile(commands[i]);
                c = new container();
                c->type = DOWHILE;
                c->obj = dw;
                w->commands.push_back(c);
                break;
            case FOR:
                fr = readFor(commands[i]);
                c = new container();
                c->type = FOR;
                c->obj = fr;
                w->commands.push_back(c);
                break;
            case PRINTF:
                prt = readPrintf(commands[i]);
                c = new container();
                c->type = PRINTF;
                c->obj = prt;
                w->commands.push_back(c);
                AST->printF.push_back(c);
                break;
            case SCANF:
                scf = readScanf(commands[i]);
                c = new container();
                c->type = SCANF;
                c->obj = scf;
                w->commands.push_back(c);
                break;
            default: 
                if(AST->symbTable[getObjType(commands[i])] != NULL) { //encontrado na tabela de simbolos
                    if(AST->symbTable[getObjType(commands[i])]->type = FUNCTION){ //chamada de funcao encontrada
                        opl = readOpLeaf(commands[i]);
                        c = new container();
                        c->type = OPLEAF;
                        c->obj = opl;
                        w->commands.push_back(c);
                    }
                }
                break;
        }
    }
    return w;
}


    

DoWhile* readDoWhile(std::string str) {
    cout << "read dowhile\n";
    DoWhile *dw = new DoWhile(), *dw2;
    While *w;
    Operation *op;
    OpLeaf *opl;
    container *c;
    Return *r;
    If *iff;
    Printf *prt;
    Scanf *scf;
    For* fr;
    int i;
    std::vector<std::string> arg, commands;


    dw->idLabel = contDoWhile;
    contDoWhile++;
    arg = splitComma(str);
    //conditions
    //command list
    if(getNum(getObjType(arg[1])) == OPERATION) {//operacao
        op = readOperation(arg[1]);
        c = new container();
        c->type = OPERATION;
        c->obj = op;
        dw->condition = c;
    }else{//opleaf
        opl = readOpLeaf(arg[1]);
        c = new container();
        c->type = OPLEAF;
        c->obj = opl;
        dw->condition = c;
    }

    commands = splitSemiCollon(arg[0]);
    for(i = 0; i < commands.size(); i++) {
        switch(getNum(getObjType(commands[i]))){
            case RETURN:
                r = readReturn(commands[i]);
                c = new container();
                c->type = RETURN;
                c->obj = r;
                dw->commands.push_back(c);
                break;
            case OPERATION:
                op = readOperation(commands[i]);
                c = new container();
                c->type = OPERATION;
                c->obj = op;
                dw->commands.push_back(c);
                break;
            case IF:
                iff = readIf(commands[i]);
                c = new container();
                c->type = IF;
                c->obj = iff;
                dw->commands.push_back(c);
                break;
            case WHILE:
                w = readWhile(commands[i]);
                c = new container();
                c->type = WHILE;
                c->obj = w;
                dw->commands.push_back(c);
                break;
            case DOWHILE:
                dw2 = readDoWhile(commands[i]);
                c = new container();
                c->type = DOWHILE;
                c->obj = dw2;
                dw->commands.push_back(c);
                break;
            case FOR:
                fr = readFor(commands[i]);
                c = new container();
                c->type = FOR;
                c->obj = fr;
                dw->commands.push_back(c);
                break;
            case PRINTF:
                prt = readPrintf(commands[i]);
                c = new container();
                c->type = PRINTF;
                c->obj = prt;
                dw->commands.push_back(c);
                AST->printF.push_back(c);
                break;
            case SCANF:
                scf = readScanf(commands[i]);
                c = new container();
                c->type = SCANF;
                c->obj = scf;
                dw->commands.push_back(c);
                break;
            default: 
                if(AST->symbTable[getObjType(commands[i])] != NULL) { //encontrado na tabela de simbolos
                    if(AST->symbTable[getObjType(commands[i])]->type = FUNCTION){ //chamada de funcao encontrada
                        opl = readOpLeaf(commands[i]);
                        c = new container();
                        c->type = OPLEAF;
                        c->obj = opl;
                        dw->commands.push_back(c);
                    }
                }
                break;
        }
    }
    return dw;
}

For* readFor(std::string str) {
    cout << "read for\n";
    For *fr = new For(), *fr2;
    DoWhile *dw;
    While *w;
    Operation *op;
    OpLeaf *opl;
    container *c;
    Return *r;
    If *iff;
    Printf *prt;
    Scanf *scf;
    int i;
    std::vector<std::string> arg, commands;

    fr->idLabel = contFor;
    contFor++;
    
    arg = splitComma(str);

    //inicializacao
    if(arg[0] != "") {
        if(getNum(getObjType(arg[0])) == OPERATION) {//operacao
            op = readOperation(arg[0]);
            c = new container();
            c->type = OPERATION;
            c->obj = op;
            fr->init = c;
        }else{//opleaf
            opl = readOpLeaf(arg[0]);
            c = new container();
            c->type = OPLEAF;
            c->obj = opl;
            fr->init = c;
        }
    }
    

    //condicao de parada
    if(arg[1] != "") {
        if(getNum(getObjType(arg[1])) == OPERATION) {//operacao
            op = readOperation(arg[1]);
            c = new container();
            c->type = OPERATION;
            c->obj = op;
            fr->condition = c;
        }else{//opleaf
            opl = readOpLeaf(arg[1]);
            c = new container();
            c->type = OPLEAF;
            c->obj = opl;
            fr->condition = c;
        }
    }

    //ajuste de valores
    if(arg[2] != "") {
        if(getNum(getObjType(arg[2])) == OPERATION) {//operacao
            op = readOperation(arg[2]);
            c = new container();
            c->type = OPERATION;
            c->obj = op;
            fr->adjustment = c;
        }else{//opleaf
            opl = readOpLeaf(arg[2]);
            c = new container();
            c->type = OPLEAF;
            c->obj = opl;
            fr->adjustment = c;
        }
    }

    //comandos

    commands = splitSemiCollon(arg[3]);
    for(i = 0; i < commands.size(); i++) {
        switch(getNum(getObjType(commands[i]))){
            case RETURN:
                r = readReturn(commands[i]);
                c = new container();
                c->type = RETURN;
                c->obj = r;
                fr->commands.push_back(c);
                break;
            case OPERATION:
                op = readOperation(commands[i]);
                c = new container();
                c->type = OPERATION;
                c->obj = op;
                fr->commands.push_back(c);
                break;
            case IF:
                iff = readIf(commands[i]);
                c = new container();
                c->type = IF;
                c->obj = iff;
                fr->commands.push_back(c);
                break;
            case WHILE:
                w = readWhile(commands[i]);
                c = new container();
                c->type = WHILE;
                c->obj = w;
                fr->commands.push_back(c);
                break;
            case DOWHILE:
                dw = readDoWhile(commands[i]);
                c = new container();
                c->type = DOWHILE;
                c->obj = dw;
                fr->commands.push_back(c);
                break;
            case FOR:
                fr2 = readFor(commands[i]);
                c = new container();
                c->type = FOR;
                c->obj = fr2;
                fr->commands.push_back(c);
                break;
            case PRINTF:
                prt = readPrintf(commands[i]);
                c = new container();
                c->type = PRINTF;
                c->obj = prt;
                fr->commands.push_back(c);
                AST->printF.push_back(c);
                break;
            case SCANF:
                scf = readScanf(commands[i]);
                c = new container();
                c->type = SCANF;
                c->obj = scf;
                fr->commands.push_back(c);
                break;
            default: 
                if(AST->symbTable[getObjType(commands[i])] != NULL) { //encontrado na tabela de simbolos
                    if(AST->symbTable[getObjType(commands[i])]->type = FUNCTION){ //chamada de funcao encontrada
                        opl = readOpLeaf(commands[i]);
                        c = new container();
                        c->type = OPLEAF;
                        c->obj = opl;
                        fr->commands.push_back(c);
                    }
                }
                break;
        }
    }
    return fr;
}   



Function* readFunction() {
    cout << "read function\n";
    container *cont;
    Function *f = new Function();
    Parameter *p;   
    Variable *v;
    If *iff;
    Return *r;
    Printf *prt;
    Scanf *scf;
    Operation *op;
    OpLeaf *opl;
    While *w;
    DoWhile *dw;
    For *fr;
    CURRENTFUNCTION = f;
    f->id = split(LINE)[1];  //function id

    LINE = readInput();
    f->return_type = split(LINE)[1]; // function return type
    

    cont = new container();
    cont->type = FUNCTION;
    cont->obj = f;
    AST->symbTable;
    AST->symbTable[f->id] = cont; //add to global symbol table

    LINE = readInput();
    while(LINE != "END_FUNCTION") {
         if(LINE != ""){
            switch(getNum(getObjType(LINE))) {
                case PARAMETER:
                    p = readParameter();
                    cont = new container();
                    cont->type = PARAMETER;
                    cont->obj = p;
                    f->param.push_back(cont);
                    f->symbTable[p->id] = cont;
                    break;
                case VARIABLE:
                    v = readVariable();
                    cont = new container();
                    cont->type = VARIABLE;
                    cont->obj = v;
                    f->symbTable[v->id] = cont;
                    break;
                case OPERATION:
                    op = readOperation(LINE);
                    cont = new container();
                    cont->type = OPERATION;
                    cont->obj = op;
                    f->commands.push_back(cont);
                    break;
                case IF:
                    iff = readIf(LINE);
                    cont = new container();
                    cont->type = IF;
                    cont->obj = iff;
                    f->commands.push_back(cont);
                    break;
                case WHILE:
                    w = readWhile(LINE);
                    cont = new container();
                    cont->type = WHILE;
                    cont->obj = w;
                    f->commands.push_back(cont);
                    break;
                case DOWHILE:
                    dw = readDoWhile(LINE);
                    cont = new container();
                    cont->type = DOWHILE;
                    cont->obj = dw;
                    f->commands.push_back(cont);
                    break;
                case FOR:
                    fr = readFor(LINE);
                    cont = new container();
                    cont->type = FOR;
                    cont->obj = fr;
                    f->commands.push_back(cont);
                    break;
                case PRINTF:
                    prt = readPrintf(LINE);
                    cont = new container();
                    cont->type = PRINTF;
                    cont->obj = prt;
                    f->commands.push_back(cont);
                    AST->printF.push_back(cont);
                    break;
                case SCANF:
                    scf = readScanf(LINE);
                    cont = new container();
                    cont->type = SCANF;
                    cont->obj = scf;
                    f->commands.push_back(cont);
                    break;
                case RETURN:
                    r = readReturn(LINE);
                    cont = new container();
                    cont->type = RETURN;
                    cont->obj = r;
                    f->commands.push_back(cont);
                    break;
                default: 
                    if(AST->symbTable[getObjType(LINE)] != NULL) { //encontrado na tabela de simbolos
                        if(AST->symbTable[getObjType(LINE)]->type = FUNCTION){ //chamada de funcao encontrada
                            opl = readOpLeaf(LINE);
                            cont = new container();
                            cont->type = OPLEAF;
                            cont->obj = opl;
                            w->commands.push_back(cont);
                        }
                    }
                    break;
            }
         }
        LINE = readInput();
    }
    CURRENTFUNCTION = NULL;
    
    return f;
}




Parameter* readParameter() {
    cout << "read parameter\n";
    Parameter *p = new Parameter();
    std::vector<std::string> splitLine = split(LINE);
    p->id = splitLine[1];
    p->type = splitLine[3];
    return p;
}

If* readIf(std::string str) {
    cout << "read if\n";
    If *iff = new If(), *iff2;
    container *c;
    Operation *op;
    OpLeaf *opl;
    Return *r;
    Printf *prt;
    std::vector<std::string> eThen, eElse, arg;
    int i = 0;

    iff->idLabel = contIf;
    contIf++;
    arg = splitComma(str);
    //read expression
    if(getNum(getObjType(arg[0])) == OPERATION) {
        op = readOperation(arg[0]);
        c = new container();
        c->type = OPERATION;
        c->obj = op;
        iff->exp.push_back(c);
    }else{
        opl = readOpLeaf(arg[0]);
        c = new container();
        c->type = OPLEAF;
        c->obj = opl;
        iff->exp.push_back(c);
    }
    

    //read all exp from then

    eThen = splitSemiCollon(arg[1]);
    for(i = 0; i < eThen.size(); i++) {
        switch(getNum(getObjType(eThen[i]))){
            case RETURN:
                r = readReturn(eThen[i]);
                c = new container();
                c->type = RETURN;
                c->obj = r;
                iff->then.push_back(c);
                break;
            case OPERATION:
                op = readOperation(eThen[i]);
                c = new container();
                c->type = OPERATION;
                c->obj = op;
                iff->then.push_back(c);
                break;
            case IF:
                iff2 = readIf(eThen[i]);
                c = new container();
                c->type = IF;
                c->obj = iff2;
                iff->then.push_back(c);
                break;
            case PRINTF:
                prt = readPrintf(eThen[i]);
                c = new container();
                c->type = PRINTF;
                c->obj = prt;
                iff->then.push_back(c);
                AST->printF.push_back(c);
                break;
            default:
                break;
        }
    }

    //read all exp from else

    if(arg.size() > 2) {
        eElse = splitSemiCollon(arg[2]);
        for(i = 0; i < eElse.size(); i++) {
            switch(getNum(getObjType(eElse[i]))){
                case RETURN:
                    r = readReturn(eElse[i]);
                    c = new container();
                    c->type = RETURN;
                    c->obj = r;
                    iff->els.push_back(c);
                    break;
                case OPERATION:
                    op = readOperation(eElse[i]);
                    c = new container();
                    c->type = OPERATION;
                    c->obj = op;
                    iff->els.push_back(c);
                    break;
                case IF:
                    iff2 = readIf(eElse[i]);
                    c = new container();
                    c->type = IF;
                    c->obj = iff2;
                    iff->els.push_back(c);
                    break;
                case PRINTF:
                    prt = readPrintf(eElse[i]);
                    c = new container();
                    c->type = PRINTF;
                    c->obj = prt;
                    iff->els.push_back(c);
                    AST->printF.push_back(c);
                    break;
                default:
                    break;
            }
        }
    }
    return iff;
}

Operation* readOperation(std::string arg) {
    cout << "read operation\n";
    Operation *op = new Operation, *op2;
    OpLeaf *opl;
    container *c;
    Return *r;
    Function *f;
    std::vector<std::string> param = splitOperation(arg);
    std::string vname;
    op->opType = getOpType(param[0]); 
    op->opGroup = getOpGroup(param[0]);
    
    switch(getNum(getObjType(param[1]))) {
        case OPERATION: 
            op2 = readOperation(param[1]);
            c = new container();
            c->type = OPERATION;
            c->obj = op2;
            op->left = c;
            break;
        case RETURN:
            r = readReturn(param[1]);
            c = new container();
            c->type = RETURN;
            c->obj = r;
            op->left = c;
            break;
        default: // variavel, constante ou funcao
            //opleaf
            opl = readOpLeaf(param[1]);
            c = new container();
            c->type = OPLEAF;
            c->obj = opl;
            op->left = c;
            break;
    };

    if(param.size() > 2) {
        switch(getNum(getObjType(param[2]))) {
            case OPERATION: 
                op2 = readOperation(param[2]);
                c = new container();
                c->type = OPERATION;
                c->obj = op2;
                op->right = c;
                break;
            case RETURN:
                r = readReturn(param[2]);
                c = new container();
                c->type = RETURN;
                c->obj = r;
                op->right = c;
                break;
            default: // variavel, constante ou funcao
                opl = readOpLeaf(param[2]);
                c = new container();
                c->type = OPLEAF;
                c->obj = opl;
                op->right = c;
                break;
        };
    }
    return op;
}


OpLeaf* readOpLeaf(std::string arg) {
    cout << "read opleaf " << arg << "\n";
    OpLeaf *opl = new OpLeaf(), *opl2;
    Operation *op;
    Variable *v;
    Parameter *p;
    std::string vname;
    container *c;
    Function *f;
    std::vector<std::string> param;
    int i;
    
    vname = splitOperation(arg)[0];
    c = AST->symbTable[vname]; //procura na global
    if(c == NULL) { //caso nao encontrar
        c = CURRENTFUNCTION->symbTable[vname]; //procura na local
    }
    if(c == NULL) { //caso nao encontrar em nenhuma das tabelas : constante
        //nao encontrado em nenhum lugar, e um valor constante
        opl->type = OP_CONSTANT;
        opl->valueType = INT;
        opl->valueId = vname;
    } else { //encontraddo em alguma das tabelas
        switch(c->type){
            case VARIABLE:
                v = (Variable *)c->obj;
                opl->type = OP_VARIABLE;
                if(v->type == "char"){
                    opl->valueType = CHAR;
                }else{
                    opl->valueType = INT;
                }
                opl->valueId = v->id;
                break;
            case PARAMETER:
                p = (Parameter *)c->obj;
                opl->type = OP_VARIABLE;
                if(p->type == "char"){
                    opl->valueType = CHAR;
                }else{
                    opl->valueType = INT;
                }
                opl->valueId = p->id;
                break;
            case FUNCTION:
                f = (Function *)c->obj;
                opl = new OpLeaf();
                opl->type = OP_FUNCTION;
                if(f->return_type == "char"){
                    opl->valueType = CHAR;
                }else {
                    opl->valueType = INT;
                }
                opl->valueId = f->id;
                param = splitOperation(arg);
                if(param.size() > 1) {
                    for(i = 1; i < param.size(); i++) {
                        switch(getNum(getObjType(param[i]))) {
                            case OPERATION:
                                op = readOperation(param[i]);
                                c = new container();
                                c->type = OPERATION;
                                c->obj = op;
                                opl->values.push_back(c);
                                break;
                            default: // opleaf
                            opl2 = readOpLeaf(param[i]);
                            c = new container;
                            c->type = OPLEAF;
                            c->obj = opl2;
                            opl->values.push_back(c);
                        };
                    }
                }
                break;
            default:
                break;
        }
    }
    return opl;
}

Printf* readPrintf(std::string arg) {
    cout << "read printf\n";
    Printf *prt = new Printf();
    container *c;
    Operation *op;
    OpLeaf *opl;
    int i;
    prt->idLabel = contPrintf;
    contPrintf++;
    std::vector<std::string> param = splitOperation(arg);
    prt->str = param[1];
    for(i = 2; i < param.size(); i++) {
        switch (getNum(getObjType(param[i]))) {
         case OPERATION: 
            op = readOperation(param[i]);
            c = new container();
            c->type = OPERATION;
            c->obj = op;
            prt->exp.push_back(c);
            break;
        default: // variavel, constante ou funcao
            //opleaf
            opl = readOpLeaf(param[i]);
            c = new container();
            c->type = OPLEAF;
            c->obj = opl;
            prt->exp.push_back(c);
            break;
        }
    }
    return prt;
}

Scanf* readScanf(std::string arg) {
    cout << "read scanf\n";
    Scanf *scf = new Scanf();
    container *c;
    Operation *op;
    OpLeaf *opl;
    std::vector<std::string> param = splitOperation(arg);

    scf->str = param[1];
    switch (getNum(getObjType(param[2]))) {
        case OPERATION: 
        op = readOperation(param[2]);
        c = new container();
        c->type = OPERATION;
        c->obj = op;
        scf->address = c;
        break;
    default: // variavel, constante ou funcao
        //opleaf
        opl = readOpLeaf(param[2]);
        c = new container();
        c->type = OPLEAF;
        c->obj = opl;
        scf->address = c;
        break;
    }
    return scf;
}

Return* readReturn(std::string arg) {
    cout << "read return\n";
    Return *r = new Return();
    Operation *op;
    OpLeaf *opl;
    container *c;
    std::vector<std::string> v = splitOperation(arg), vleaf;
    std::string param;
    if(v.size() > 1){
        param = splitOperation(arg)[1];
        if(getNum(getObjType(param)) == OPERATION) {
            op = readOperation(param);
            c = new container();
            c->type = OPERATION;
            c->obj = op;
            r->exp = c;
        }else {
            opl = readOpLeaf(param);
            c = new container();
            c->type = OPLEAF;
            c->obj = opl;
            r->exp = c;
        }
    }
    return r;
}

Variable* readVariable() {
    cout << "read variable\n";
    Variable *v = new Variable();
    std::vector<std::string> splitLine = split(LINE);
    v->id = splitLine[1];
    v->type = splitLine[3];
    return v;
}

int main() {
    readProgram();
    /*cout<<"\n\n\n\n Código gerado: \n\n\n\n";
    AST->print();*/


    return 1;
}


