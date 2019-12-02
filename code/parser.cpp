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
Function* readFunction();
Parameter* readParameter();
If* readIf();
Operation* readOperation(std::string arg);
OpLeaf* readOpLeaf(std::string arg);
Return* readReturn(std::string arg);
Variable* readVariable();
Printf* readPrintf(std::string arg);
Scanf* readScanf(std::string arg);
int FILEEND = 0;
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
    while(!(c >= 65 && c <= 90) && !(c >= 97 && c <= 122) && !(c >= '0' && c <= '9') &&  
            c != '+' && c != '-' && c != '*' && c != '/' && c != '%' && c != '&' && c != '|' && 
            c != '^' && c != '=' && c != '!' && c != '<' && c != '>' && c != '-' && c != '~') {
        i++;
        if(i >= line.size()) {
            break;
        }
        c = line.at(i);
    }
    c = line.at(i);
    while((c >= 65 && c <= 90) || (c >= 97 && c <= 122) && !(c >= '0' && c <= '9') ||  
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
    if(str == "GLOBALVARIABLE") return GLOBALVARIABLE;
    if(str == "FUNCTION") return FUNCTION;
    if(str == "PARAMETER") return PARAMETER;
    if(str == "VARIABLE") return VARIABLE;
    if(str == "DOWHILE") return DOWHILE;
    if(str == "IF") return IF;
    if(str == "WHILE") return WHILE;
    if(str == "FOR") return FOR;
    if(str == "PRINTF") return PRINTF;
    if(str == "SCANF") return SCANF;
    if(str == "EXIT") return EXIT;
    if(str == "RETURN") return RETURN;
    if(str == "<=") return OPERATION; //TODO
    if(str == "==") return OPERATION;
    if(str == "*") return OPERATION;
    if(str == "-") return OPERATION;
    if(str == "&") return OPERATION;
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
    int i = 0;

    while(i < str.size()) {
        c = str.at(i);
        if(c == ';'){
            tokens.push_back(s);
            s = "";
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

std::vector<std::string> splitAmpersand(std::string str) {
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
                case IF:
                    iff = readIf();
                    cont = new container();
                    cont->type = IF;
                    cont->obj = iff;
                    f->commands.push_back(cont);
                    break;
                case PRINTF:
                    prt = readPrintf(LINE);
                    cont = new container();
                    cont->type = PRINTF;
                    cont->obj = prt;
                    f->commands.push_back(cont);
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

If* readIf() {
    cout << "read if\n";
    If *iff = new If();
    container *c;
    Operation *op;
    Return *r;
    std::vector<std::string> eThen, eElse, arg;
    int i = 0;
    arg = splitComma(LINE);
    //read expression
    op = readOperation(arg[0]);
    c = new container();
    c->type = OPERATION;
    c->obj = op;
    iff->exp.push_back(c);

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
    op->opType = getNum(param[0]);
    
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
    
    //verificar quantidade de filhos
    //para cada filho, verificar o que é
        //se operacao, chamada recursiva
        // caso nao seja, verificar as tabelas de simbolo.
            //se encontrado, adicionar com o tipo encontrado
            //se nao for enconrtado, e um valor constante




    //verificar cada filho
    //criar as folhas
    //adicionar ao objeto


    return op;
}


OpLeaf* readOpLeaf(std::string arg) {
    cout << "read opleaf\n";
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
    std::vector<std::string> v = splitOperation(arg), vleaf;
    std::string param;
    if(v.size() > 1){
        param = splitOperation(arg)[1];
        if(getNum(getObjType(param)) == OPERATION) {
            Operation *op = readOperation(param);
            container *c = new container();
            c->type = OPERATION;
            c->obj = op;
            r->exp = c;
        }else {
            vleaf = splitOperation(param);
            for(int i = 0; i < vleaf.size(); i++) {
            }
            //readopleaf
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

/*
void testOperation(){


   OpLeaf *leftLeaf1 = new OpLeaf();
        leftLeaf1->type = OP_VARIABLE;
        leftLeaf1->valueType = INT;
        leftLeaf1->valueId = "s0";
    OpLeaf *rightLeaf1 = new OpLeaf();
        rightLeaf1->type = OP_VARIABLE;
        rightLeaf1->valueType = INT;
        rightLeaf1->valueId = "s1";

    container *leftContainer1 = new container();
        leftContainer1->type=OPLEAF;
        leftContainer1->obj = leftLeaf1;
    container *rightContainer1 = new container();
        rightContainer1->type=OPLEAF;
        rightContainer1->obj = rightLeaf1;

    Operation *leftOp = new Operation();
        leftOp->opType = OP_ADD; // add
        leftOp->left = leftContainer1;
        leftOp->right = rightContainer1;

    container *leftOpContainer = new container();
        leftOpContainer->type = OPERATION;
        leftOpContainer->obj = leftOp;

    OpLeaf *rightLeaf = new OpLeaf();
        rightLeaf->type = OP_VARIABLE;
        rightLeaf->valueType = INT;
        rightLeaf->valueId = "s3";

    container *rightContainer = new container();
        rightContainer->type=OPLEAF;
        rightContainer->obj = rightLeaf;

    Operation root = Operation();
    root.opType = OP_ADD; // add
    root.left = leftOpContainer;
    root.right = rightContainer;
    root.print();
    std::cout<<"-------------------------"<<std::endl;
    
*/

    //std::vector<std::string> v = splitSemiCollon("=(a[i],j))");
    //std::vector<std::string> v = splitComma("FOR(=(i,0),<(i,max),(i)++,PRINTF(\"Entre com o valor da posicao %d: \",+(i,1));SCANF(\"%d\",&(j));=(a[i],j));");
    //std::vector<std::string> v = splitOperation("=(a[i],j,fat(1, 2))");
    //std::vector<std::string> v = splitComma("=(i,0)");
    //std::vector<std::string> v = splitOperation("return(0)");

    /*
    for(int i = 0; i < v.size(); i++) {
        cout << v[i] << "\n";
    }
    */



}

/*


FOR(=(i,0),
<(i,max),
(i)++,

PRINTF("Entre com o valor da posicao %d: ",+(i,1));
SCANF("%d",&(j));
=(a[i],j));


*/