#include <iostream>
#include <string>
#include <vector>
#include "classes.h"

using namespace std;


int FILEEND = 0;
std::string LINE;
Program AST;


std::string readInput() {
    char c;
    std::string str;
    int n = 0;

    while(scanf("%c", &c)!=EOF){
        if(c == '\n'){n = 1; break;}
        str.append(1, c);
    }

    if(!n){FILEEND = 1;}

    return str;
}

std::string getObjType (std::string line) {
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
    if(str == "VARIABLE") return VARIABLE;
    if(str == "DOWHILE") return DOWHILE;
    if(str == "IF") return IF;
    if(str == "WHILE") return WHILE;
    if(str == "FOR") return FOR;
    if(str == "PRINTF") return PRINTF;
    if(str == "SCANF") return SCANF;
    if(str == "EXIT") return EXIT;
    if(str == "RETURN") return RETURN;
    if(str == "BOP") return BOP;
    if(str == "UOP") return UOP;

    return 0;
}

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



void readProgram() {
}

Function* readFunction() {
    Function *f = new Function();
    f->id = split(LINE)[1];  //function id
    LINE = readInput();
    f->return_type = split(LINE)[1]; // function return type
    //read variables
    //read commands
    
    container *c = new container();
    c->type = FUNCTION;
    c->obj = f;
    AST.functions.push_back(*c);
    return f;
    
}
int main() {
    Function f;
    container *c;
    LINE = readInput();
    while(FILEEND != 1) {
        if(LINE != ""){
            cout << LINE << '\n';
            switch(getNum(getObjType(LINE))) {
                case FUNCTION:
                    readFunction();
                    //c = new container();
                    cout << "\n\n[" << ((Function *)(AST.functions[0].obj))->id << "]\n\n";
                    break;

                default:
                break;
            }
        }
        LINE = readInput();
    }
}
