#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct _container{
    int type;
    void *obj;
} container;

class Constant {
    public:
        int objType = 0;
        std::string id;
        std::string value;
};

class GlobalVariable {
    public:
        int objType = 1;
        std::string id;
        std::string type;
};

class Function {
    public:
        int objType = 2;
        std::string id;
        std::string return_type;
        std::string commands;
};

class Variable {
    public:
        int objType = 3;
        std::string id;
        std::string type;
};

class DoWhile {
    public:
        int objType = 4;
        std::vector<container> commands;
        container condition;
};

class If {
    public:
        int objType = 5;
        container exp;
        std::vector<container> then;
        std::vector<container> els;
};

class While {
    public:
        int objType = 6;
        container condition;
        std::vector<container> commands;
};

class For {
    public:
        int objType = 7;
        container init;
        container condition;
        container adjustment;
        std::vector<container> commands;
};

class Printf {
    public:
        int objType = 8;
        std::string str;
        std::vector<container> exp;
};

class Scanf {
    public:
        int objType = 9;
        std::string str;
        container address;
};

class Exit {
    public:
        int objType = 10;
        container exp;
};

class Return {
    public:
        int objType = 11;
        container exp;
};

class Bop {
    public:
        int objType = 12;
        container left;
        container right;
};

class Uop {
    public:
        int objType = 13;
        container exp;
};

int main() { 
    cout << "penis";
}