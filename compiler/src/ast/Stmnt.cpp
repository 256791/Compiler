#include "AST.h"
#include <iostream>

const string Stmnt::NAME = "Statement";
const string CompoundStmnt::NAME = "CompoundStatement";
const string Syscall::NAME = "SystemCall";

Stmnt::Stmnt(int l): lineno(l){}

CompoundStmnt::CompoundStmnt(int l, Stmnt *stmnt): Stmnt(l)
{
    this->stmnts.push_back(stmnt);
}

CompoundStmnt::CompoundStmnt(int l, CompoundStmnt *stmnts, Stmnt *stmnt): Stmnt(l)
{
    this->stmnts.insert(this->stmnts.end(), stmnts->stmnts.begin(), stmnts->stmnts.end());
    this->stmnts.push_back(stmnt);
    delete stmnts;
}

Syscall::Syscall(int l, Stmnt *val, char type): Stmnt(l)
{
    this->type = type;
    this->val = dynamic_cast<Expr *>(val);
}

vector<RTLNode*> CompoundStmnt::toRTL()
{
    vector<RTLNode*> nodes;
    for(Stmnt* s: this->stmnts){
        try{
            vector<RTLNode*> n = s->toRTL();
            nodes.insert(end(nodes), begin(n), end(n));
        }
        catch(...){
            cout << ":(\n";
            cerr << s->NAME << endl;
        }
    }
    return nodes;
}

vector<RTLNode*> Syscall::toRTL()
{
    vector<RTLNode*> nodes;
    RTLNode* node = new Call(this->type=='R'?"GET":"PUT", dynamic_cast<RTLObject* >(this->val->toRTL()[0]));
    nodes.push_back(node);
    return nodes;
}

bool Syscall::checkVariables(vector<tuple<string,bool> > &variables,vector<string> iterators){
    if(this->type == 'R'){
        for (auto v : iterators)
        {
            if (ArrDecl *arr = dynamic_cast<ArrDecl *>(this->val))
            {
                if (v == arr->name)
                {
                    cout << "\n\033[31mError\033[0m Can't modify iterator "
                        << arr->name << " at line " << this->lineno << "\n";
                    return false;
                }
            }else if (VarRef *var = dynamic_cast<VarRef *>(this->val))
            {
                if (v == var->name)
                {
                    cout << "\n\033[31mError\033[0m Can't modify iterator "
                        << var->name << " at line " << this->lineno << "\n";
                    return false;
                }
            }
        }
    }
    return this->val->checkVariables(variables, iterators);
}

bool CompoundStmnt::checkVariables(vector<tuple<string,bool> > &variables,vector<string> iterators){
    for(auto stmnt : this->stmnts){
        if(!stmnt->checkVariables(variables, iterators))
            return false;
    }
    return true;
}