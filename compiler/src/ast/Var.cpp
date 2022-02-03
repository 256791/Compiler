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

bool VarDecl::checkVariables(vector<tuple<string,bool,bool>> &variables,vector<string> iterators){
    return true;
}

bool VarConst::checkVariables(vector<tuple<string,bool,bool>> &variables,vector<string> iterators){
    return true;
}

bool ArrDecl::checkVariables(vector<tuple<string,bool,bool>> &variables,vector<string> iterators){
    return true;
}

bool ArrRef::checkVariables(vector<tuple<string,bool,bool>> &variables,vector<string> iterators){
    bool flag = true;
    for(auto v : variables){
        if(get<0>(v) == this->name){
            if(!get<2>(v)){
                cout << "\n\033[31mError\033[0m Use of uninitialized array variable "
                << this->name << " at line " << this->lineno << "\033[0m\n";
                return false;
            }

            if(get<1>(v)){
                flag = false;
                break;
            }
            cout << "\n\033[31mError\033[0m Wrong use of variable "
                << this->name << " at line " << this->lineno << "\033[0m\n";
            return false;
        }
    }


    if(flag){
        cout << "\n\033[31mError\033[0m Undefinded array variable "
        << this->name << " at line " << this->lineno << "\033[0m\n";
        return false;
    }
    return this->at->checkVariables(variables, iterators);
}

bool VarRef::checkVariables(vector<tuple<string,bool,bool>> &variables,vector<string> iterators){
    for(auto v : variables){
        if(get<0>(v) == this->name){
            if(!get<2>(v)){
                cout << "\n\033[31mError\033[0m Use of uninitialized variable "
                << this->name << " at line " << this->lineno << "\033[0m\n";
                return false;
            }

            if(!get<1>(v)){
                return true;
            }
            cout << "\n\033[31mError\033[0m Wrong use of array variable "
                << this->name << " at line " << this->lineno << "\033[0m\n";
            return false;
        }
    }
    cout << "\n\033[31mError\033[0m Undefinded variable "
        << this->name << " at line " << this->lineno << "\033[0m\n";
    return false;
}