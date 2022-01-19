#include "AST.h"
#include <iostream>

const string Stmnt::NAME = "Statement";
const string CompoundStmnt::NAME = "CompoundStatement";
const string Syscall::NAME = "SystemCall";

CompoundStmnt::CompoundStmnt(Stmnt *stmnt)
{
    this->stmnts.push_back(stmnt);
}

CompoundStmnt::CompoundStmnt(CompoundStmnt *stmnts, Stmnt *stmnt)
{
    this->stmnts.insert(this->stmnts.end(), stmnts->stmnts.begin(), stmnts->stmnts.end());
    this->stmnts.push_back(stmnt);
    delete stmnts;
}

Syscall::Syscall(Stmnt *val, char type)
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