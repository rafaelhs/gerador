#include <iostream>
#include <string>
#include <vector>
#include "classes.h"
using namespace std;

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



// int main(){
//     arithmeticOperation();
// }