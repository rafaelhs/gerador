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


void arithmeticOperation(){
    bool regT[10] = {false,false,false,false,false,false,false,false,false,false};
    
    OpLeaf *lRightLOP = new OpLeaf();
        lRightLOP->type = OP_VARIABLE;
        lRightLOP->valueType = INT;
        lRightLOP->valueId = "s0";
    OpLeaf *r_LOP = new OpLeaf();
        r_LOP->type = CONSTANT;
        r_LOP->valueType = INT;
        r_LOP->valueId = "1";

    container *lRightLOPC = new container();
        lRightLOPC->type= OPLEAF;
        lRightLOPC->obj = lRightLOP;

    container *r_LOPC = new container();
        r_LOPC->type= OPLEAF;
        r_LOPC->obj = r_LOP;






  OpLeaf *var = new OpLeaf();
        var->type = OP_VARIABLE;
        var->valueType = INT;
        var->valueId = "s4";

    container *varC = new container();
        varC->type= OPLEAF;
        varC->obj = var;



    Operation *add = new Operation();
        add->opType= OP_ADD;
        add->opGroup=ARITHMETICAL;
        add->left = lRightLOPC;
        add->right = r_LOPC;

    container *addC = new container();
        addC->type= OPERATION;
        addC->obj = add;
    
    Operation *assign = new Operation();
        assign->opType= OP_ASSIGN;
        assign->opGroup=ARITHMETICAL;
        assign->left = varC;
        assign->right = addC;


        
    assign->print(regT);
    


}

void logicalOperation(){
    bool regT[10] = {false,false,false,false,false,false,false,false,false,false};

   OpLeaf *l_init = new OpLeaf();
        l_init->type = OP_VARIABLE;
        l_init->valueType = INT;
        l_init->valueId = "s0";
    OpLeaf *r_init = new OpLeaf();
        r_init->type = OP_VARIABLE;
        r_init->valueType = INT;
        r_init->valueId = "s1";

    container *leftInitContainer = new container();
        leftInitContainer->type= OPLEAF;
        leftInitContainer->obj = l_init;

    container *rightInitContainer = new container();
        rightInitContainer->type= OPLEAF;
        rightInitContainer->obj = r_init;



    Operation *init = new Operation();
        init->opGroup = ARITHMETICAL;
        init->opType = OP_ASSIGN;
        init->left = leftInitContainer;
        init->right = rightInitContainer;

    container *initc = new container();
        initc->obj = init;
        initc->type= OPERATION;



    OpLeaf *leftAdd = new OpLeaf();
        leftAdd->type = OP_VARIABLE;
        leftAdd->valueType = INT;
        leftAdd->valueId = "s0";
    OpLeaf *rightAdd = new OpLeaf();
        rightAdd->type = OP_VARIABLE;
        rightAdd->valueType = INT;
        rightAdd->valueId = "s1";

    container *leftAddC = new container();
        leftAddC->type= OPLEAF;
        leftAddC->obj = leftAdd;

    container *rightAddC = new container();
        rightAddC->type= OPLEAF;
        rightAddC->obj = rightAdd;



    Operation *add = new Operation();
        add->opType= OP_ADD;
        add->opGroup=ARITHMETICAL;
        add->left = leftAddC;
        add->right = rightAddC;

    container *addC = new container();
        addC->type= OPERATION;
        addC->obj = add;








    For *dw = new For();
    // DoWhile *dw = new DoWhile();
    // While *dw = new While();
   OpLeaf *l_LOP = new OpLeaf();
        l_LOP->type = OP_VARIABLE;
        l_LOP->valueType = INT;
        l_LOP->valueId = "s0";
    OpLeaf *r_LOP = new OpLeaf();
        r_LOP->type = OP_VARIABLE;
        r_LOP->valueType = INT;
        r_LOP->valueId = "s1";

    container *l_LOPC = new container();
        l_LOPC->type= OPLEAF;
        l_LOPC->obj = l_LOP;

    container *r_LOPC = new container();
        r_LOPC->type= OPLEAF;
        r_LOPC->obj = r_LOP;



    OpLeaf *l_ROP = new OpLeaf();
        l_ROP->type = OP_VARIABLE;
        l_ROP->valueType = INT;
        l_ROP->valueId = "s2";
    OpLeaf *r_ROP = new OpLeaf();
        r_ROP->type = OP_VARIABLE;
        r_ROP->valueType = INT;
        r_ROP->valueId = "s3";

    container *l_ROPC = new container();
        l_ROPC->type= OPLEAF;
        l_ROPC->obj = l_ROP;

    container *r_ROPC = new container();
        r_ROPC->type= OPLEAF;
        r_ROPC->obj = r_ROP;
    
    
    Operation *lop = new Operation();
        lop->opGroup = LOGICAL;
        lop->opType = EQUAL;
        lop->left = l_LOPC;
        lop->right = r_LOPC;

    container *lopc = new container();
        lopc->obj = lop;
        lopc->type= OPERATION;


    Operation *rop = new Operation();
        rop->opGroup = LOGICAL;
        rop->opType = LESS_THAN;
        rop->left = l_ROPC;
        rop->right = r_ROPC;
    container *ropc = new container();
        ropc->obj = rop;
        ropc->type =OPERATION;



    Operation *root = new Operation();
        root->opType = LOGICAL_OR; 
        root->opGroup =LOGICAL;
        root->left = lopc;
        root->right = ropc;

    container *rCont = new container();
        rCont->type=OPERATION;
        rCont->obj = root;
    cout<<"do{\n\n\n\n}while(s0 == s1) || (s2<s3);\n"<<endl;
    dw->init = initc;
    dw->condition = rCont;
    dw->adjustment = addC;
    dw->print(regT);
}

int main() {
    // readProgram();
    // arithmeticOperation();
    logicalOperation();
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