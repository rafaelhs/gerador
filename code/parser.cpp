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
Return* readReturn(std::string arg);


int FILEEND = 0;
std::string LINE;
Program *AST;




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
    while(!(c >= 65 && c <= 90) && !(c >= 97 && c <= 122)) {
        i++;
        c = line.at(i);
    }
    c = line.at(i);
    while((c >= 65 && c <= 90) || (c >= 97 && c <= 122)){
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

std::vector<std::string> splitComma(std::string str) {
    std::vector<std::string> tokens;
    std:: string s = "";
    char c;
    int i = 0;

    while(str.at(i) != '('){
        i++;
    }
    i++;
    while(true) {
        c = str.at(i);
        if(c == ','){ //se encontra virgula adiciona a expressao no vetor
            tokens.push_back(s);
            s = "";
        }else if(c == '(') { //se encontra a abertura de parenteses continua copiando ate encontrar o fechamento
            while(c != ')'){
                s.append(1, c);
                i++;
                c = str.at(i);
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

    char c;
    int i = 0;

    c = str.at(i);
    while(c != '(') {
        s.append(1, c);
        i++;
        c = str.at(i);
    }
    tokens.push_back(s);
    s = "";
    i++;
    c = str.at(i);
    while(c != ')') {
        if(c == '(') {
            while(c != ')'){
                s.append(1, c);
                i++;
                c = str.at(i);
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
    container *cont;
    Constant *c;
    GlobalVariable *gv;
    Function *f;

    AST = new Program();


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
    container *cont;
    Function *f = new Function();
    Parameter *p;   
    Variable *v;
    If *iff;

    f->id = split(LINE)[1];  //function id
    LINE = readInput();
    f->return_type = split(LINE)[1]; // function return type
    

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
                    break;
                case VARIABLE:
                    break;
                case IF:
                    iff = readIf();
                    cont = new container();
                    cont->type = IF;
                    cont->obj = iff;
                    f->commands.push_back(cont);
                    break;
                default:
                    break;
            }
         }
        LINE = readInput();
    }


    return f;
}

Parameter* readParameter() {
    Parameter *p = new Parameter();
    std::vector<std::string> splitLine = split(LINE);
    p->id = splitLine[1];
    p->type = splitLine[3];
    return p;
}

If* readIf() {
    cout << "if\n";
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
        eElse = splitSemiCollon(arg[1]);
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

Operation* readOperation(std::string arg) { //TODO
    Operation *op = new Operation;
    vector<std::string> param = splitComma(arg);
    cout << "OP\n";
    //verificar cada filho
    //criar as folhas
    //adicionar ao objeto


    return op;
}

Return* readReturn(std::string arg) {
    cout << "RET\n";

}

void testOperation(){


   OpLeaf *leftLeaf1 = new OpLeaf();
        leftLeaf1->type = "v";
        leftLeaf1->valueType = "int";
        leftLeaf1->valueId = "a";
    OpLeaf *rightLeaf1 = new OpLeaf();
        rightLeaf1->type = "v";
        rightLeaf1->valueType = "int";
        rightLeaf1->valueId = "b";

    container *leftContainer1 = new container();
        leftContainer1->type=OPLEAF;
        leftContainer1->obj = leftLeaf1;
    container *rightContainer1 = new container();
        rightContainer1->type=OPLEAF;
        rightContainer1->obj = rightLeaf1;

    Operation *leftOp = new Operation();
        leftOp->opType = 0; // add
        leftOp->left = leftContainer1;
        leftOp->right = rightContainer1;

    container *leftOpContainer = new container();
        leftOpContainer->type = OPERATION;
        leftOpContainer->obj = leftOp;








    OpLeaf *rightLeaf = new OpLeaf();
        rightLeaf->type = "v";
        rightLeaf->valueType = "int";
        rightLeaf->valueId = "c";

    container *rightContainer = new container();
        rightContainer->type=OPLEAF;
        rightContainer->obj = rightLeaf;

    Operation root = Operation();
    root.opType = 0; // add
    root.left = leftOpContainer;
    root.right = rightContainer;
    root.print();
    std::cout<<"-------------------------"<<std::endl;
    root.opType = 1; // sub
    root.print();
    std::cout<<"-------------------------"<<std::endl;
    root.opType = 2; // mult
    root.print();
    std::cout<<"-------------------------"<<std::endl;
    root.opType = 3; // div
    root.print();
    std::cout<<"-------------------------"<<std::endl;

}

int main() {
    // readProgram();
    testOperation();
    //std::vector<std::string> v = splitSemiCollon("=(a[i],j))");
    //std::vector<std::string> v = splitComma("FOR(=(i,0),<(i,max),(i)++,PRINTF(\"Entre com o valor da posicao %d: \",+(i,1));SCANF(\"%d\",&(j));=(a[i],j));");
    //std::vector<std::string> v = splitOperation("=(a[i],j,fat(1, 2))");
    /*
    std::vector<std::string> v = splitComma("=(i,0)");
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