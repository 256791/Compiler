#include "AST.h"

const string VarDecl::NAME = "VariableDeclaration";
const string ArrDecl::NAME = "ArrayVariableDeclaration";
const string VarConst::NAME = "VariableConstans";
const string VarRef::NAME = "VariableReference";
const string ArrRef::NAME = "ArrayVariableReference";

VarDecl::VarDecl(int l, string name): Stmnt(l)
{
    this->name = name;
}

ArrDecl::ArrDecl(int l, string name, long long from, long long to): Stmnt(l)
{
    this->name = name;
    this->from = from;
    this->to = to;
    this->size = to - from + 1;
}

VarConst::VarConst(int l, long long value): Expr(l)
{
    this->value = value;
}

VarRef::VarRef(int l, string name): Expr(l)
{
    this->name = name;
}

ArrRef::ArrRef(int l, string name, Stmnt *stmnt): Expr(l)
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
    ptr->offset = this->from;
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

bool VarDecl::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}

bool VarConst::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}

bool ArrDecl::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}

bool ArrRef::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}

bool VarRef::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}