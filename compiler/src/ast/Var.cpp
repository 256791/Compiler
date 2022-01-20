#include "AST.h"

const string VarDecl::NAME = "VariableDeclaration";
const string ArrDecl::NAME = "ArrayVariableDeclaration";
const string VarConst::NAME = "VariableConstans";
const string VarRef::NAME = "VariableReference";
const string ArrRef::NAME = "ArrayVariableReference";

VarDecl::VarDecl(string name)
{
    this->name = name;
}

ArrDecl::ArrDecl(string name, int from, int to)
{
    this->name = name;
    this->from = from;
    this->to = to;
    this->size = to - from + 1;
}

VarConst::VarConst(int value)
{
    this->value = value;
}

VarRef::VarRef(string name)
{
    this->name = name;
}

ArrRef::ArrRef(string name, Stmnt *stmnt)
{
    this->name = name;
    this->at = dynamic_cast<Expr *>(stmnt);
}

vector<RTLNode*> VarDecl::toRTL()
{
    vector<RTLNode*> nodes;
    Variable* ptr = new Variable(this->name);
    nodes.push_back(ptr);
    return nodes;
}

vector<RTLNode*> ArrDecl::toRTL()
{
    vector<RTLNode*> nodes;
    Array* ptr = new Array(this->name);
    ptr->offset = this->from-1;
    ptr->size = this->size;
    nodes.push_back(ptr);
    return nodes;
}

vector<RTLNode*> VarConst::toRTL()
{
    vector<RTLNode*> nodes;
    Constans* ptr = new Constans(Constans::getNewName());
    ptr->value = this->value;
    nodes.push_back(ptr);
    return nodes;
}

vector<RTLNode*> VarRef::toRTL()
{
    vector<RTLNode*> nodes;
    nodes.push_back(new Variable(this->name));
    return nodes;
}

vector<RTLNode*> ArrRef::toRTL()
{
    vector<RTLNode*> nodes;
    Array* ptr = new Array(this->name);
    ptr->at = dynamic_cast<RTLObject* >(this->at->toRTL()[0]);
    nodes.push_back(ptr);
    return nodes;
}