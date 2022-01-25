#include "AST.h"

const string Expr::NAME = "Expression";
const string Comp::NAME = "Comparison";
const string BinOpExpr::NAME = "BinaryOperation";

Expr::Expr(int l): Stmnt(l){}

Comp::Comp(int l, string op, Stmnt *a, Stmnt *b): Expr(l)
{
    this->op = op;
    this->a = dynamic_cast<Expr *>(a);
    this->b = dynamic_cast<Expr *>(b);
}

BinOpExpr::BinOpExpr(int l, char op, Stmnt *a, Stmnt *b): Expr(l)
{
    this->op = op;
    this->a = dynamic_cast<Expr *>(a);
    this->b = dynamic_cast<Expr *>(b);
}

vector<RTLNode *> Comp::toRTL()
{
    vector<RTLNode *> nodes;

    VReg *to = new VReg(VReg::getNewName());
    RTLObject *op_a = dynamic_cast<RTLObject *>(this->a->toRTL()[0]);
    RTLObject *op_b = dynamic_cast<RTLObject *>(this->b->toRTL()[0]);
    Operation *ptr;

    ptr = new Operation(to, op_a, '-', op_b);

    nodes.push_back(ptr);
    return nodes;
}

vector<RTLNode *> Comp::toRTL(RTLNode *then)
{
    vector<RTLNode *> nodes;

    VReg *to = new VReg(VReg::getNewName());
    to->adr = RTLObject::allocateVariable();

    RTLObject *op_a = dynamic_cast<RTLObject *>(this->a->toRTL()[0]);
    RTLObject *op_b = dynamic_cast<RTLObject *>(this->b->toRTL()[0]);
    Operation *operation = new Operation(to, op_a, '-', op_b);
    nodes.push_back(operation);

    if (this->op == "==")
    {
        nodes.push_back(new Jump('P', then, to));
        nodes.push_back(new Jump('N', then, to));
    }
    else if (this->op == ">")
    {
        nodes.push_back(new Jump('N', then, to));
        nodes.push_back(new Jump('Z', then, to));
    }
    else if (this->op == ">=")
    {
        nodes.push_back(new Jump('N', then, to));
    }
    else if (this->op == "<")
    {
        nodes.push_back(new Jump('P', then, to));
        nodes.push_back(new Jump('Z', then, to));
    }
    else if (this->op == "<=")
    {
        nodes.push_back(new Jump('P', then, to));
    }
    else if (this->op == "!=")
    {
        nodes.push_back(new Jump('Z', then, to));
    }

    return nodes;
}

vector<RTLNode *> BinOpExpr::toRTL()
{
    vector<RTLNode *> nodes;

    RTLObject *op_a = dynamic_cast<RTLObject *>(this->a->toRTL()[0]);
    RTLObject *op_b;

    BinOpExpr *expr;
    if (expr = dynamic_cast<BinOpExpr *>(this->b))
    {
        Operation *operation = dynamic_cast<Operation *>(expr->toRTL()[0]);
        op_b = operation->dest;
        nodes.push_back(operation);
    }
    else
    {
        op_b = dynamic_cast<RTLObject *>(this->b->toRTL()[0]);
    }

    if (this->op == '=')
    {
        Assignment *ptr = new Assignment(op_a, op_b);
        nodes.push_back(ptr);
    }
    else
    {
        VReg *to = new VReg(VReg::getNewName());
        Operation *ptr = new Operation(to, op_a, this->op, op_b);
        nodes.push_back(ptr);
    }

    return nodes;
}

bool Comp::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}

bool BinOpExpr::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}