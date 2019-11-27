#include <iostream>
#include <string>
#include <vector>
#include "classes.h"


using namespace std;


int main() { 
    Uop i;
    cout << i.objType << '\n';
    container c;
    c.type = i.objType;
    c.obj = &i;
    i.objType = 99;
    cout << ((Uop *)(c.obj))->objType;
}