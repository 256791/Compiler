#include "AST.h"

const string IfStmnt::NAME = "IfStatement";
const string WhileStmnt::NAME = "WhileStatement";
const string DoStmnt::NAME = "DoStatement";
const string ForStmnt::NAME = "ForStatement";

IfStmnt::IfStmnt(int l, Stmnt *cond, CompoundStmnt *then): Stmnt(l)
{
    this->cond = dynamic_cast<Comp *>(cond);

    this->then = then;

    this->els = nullptr;
};

IfStmnt::IfStmnt(int l, Stmnt *cond, CompoundStmnt *then, CompoundStmnt *els): Stmnt(l)
{
    this->cond = dynamic_cast<Comp *>(cond);

    this->then = then;
    this->els = els;
};

WhileStmnt::WhileStmnt(int l, Stmnt *cond, CompoundStmnt *stmnts): Stmnt(l)
{
    this->cond = dynamic_cast<Comp *>(cond);
    this->stmnts = stmnts;
}

DoStmnt::DoStmnt(int l, Stmnt *cond, CompoundStmnt *stmnts): Stmnt(l)
{
    this->cond = dynamic_cast<Comp *>(cond);
    this->stmnts = stmnts;
    
    if(this->cond->op == "==")
        this->cond->op = "!=";
    else if(this->cond->op == "!=")
        this->cond->op = "==";
    else if(this->cond->op == "<")
        this->cond->op = ">=";
    else if(this->cond->op == ">")
        this->cond->op = "<=";
    else if(this->cond->op == "<=")
        this->cond->op = ">";
    else if(this->cond->op == ">=")
        this->cond->op = "<";
}

ForStmnt::ForStmnt(int l, string iterator, Stmnt *from, Stmnt *to, char type, CompoundStmnt *stmnts): Stmnt(l)
{
    CompoundStmnt* iterator_assignment = new CompoundStmnt(l, new BinOpExpr(l, '=', new VarRef(l, iterator), from));
    this->init = new CompoundStmnt(l, iterator_assignment, new BinOpExpr(l, '=', new VarRef(l, iterator+"_to"), to));
    
    if (type == 'D')
    {
        this->cond = new Comp(l, ">=", new VarRef(l, iterator), new VarRef(l, iterator+"_to"));
        this->after = new BinOpExpr(l, '=', new VarRef(l, iterator), new BinOpExpr(l, '-', new VarRef(l, iterator), new VarConst(l, 1)));
    }
    else
    {
        this->cond = new Comp(l, "<=", new VarRef(l, iterator), new VarRef(l, iterator+"_to"));
        this->after = new BinOpExpr(l, '=', new VarRef(l, iterator), new BinOpExpr(l, '+', new VarRef(l, iterator), new VarConst(l, 1)));
    }

    this->stmnts = stmnts;
}

vector<RTLNode*> IfStmnt::toRTL()
{
    Flag* jumpPtrElse = new Flag();
    Flag* jumpPtrEnd = new Flag();

    vector<RTLNode*> then_body = this->then->toRTL();

    vector<RTLNode*> nodes = this->cond->toRTL(jumpPtrElse);
    nodes.insert(nodes.end(), then_body.begin(), then_body.end());
    nodes.push_back(new Jump(jumpPtrEnd));
    nodes.push_back(jumpPtrElse);

    if(this->els != nullptr){
        vector<RTLNode*> else_body;
        else_body = this->els->toRTL();
        nodes.insert(nodes.end(), else_body.begin(), else_body.end());
    }

    nodes.push_back(jumpPtrEnd);
    return nodes;
}

vector<RTLNode*> WhileStmnt::toRTL()
{
    Flag* jumpPtrLoop = new Flag();
    Flag* jumpPtrEnd = new Flag();

    vector<RTLNode*> cond = this->cond->toRTL(jumpPtrEnd);
    vector<RTLNode*> body = this->stmnts->toRTL();
    
    vector<RTLNode*> nodes;
    nodes.push_back(jumpPtrLoop);
    nodes.insert(nodes.end(), cond.begin(), cond.end());
    nodes.insert(nodes.end(), body.begin(), body.end());
    nodes.push_back(new Jump(jumpPtrLoop));
    nodes.push_back(jumpPtrEnd);
    return nodes;
}

vector<RTLNode*> DoStmnt::toRTL()
{
    Flag* jumpPtrLoop = new Flag();
    Flag* jumpPtrEnd = new Flag();

    vector<RTLNode*> body = this->stmnts->toRTL();
    vector<RTLNode*> cond = this->cond->toRTL(jumpPtrEnd);

    vector<RTLNode*> nodes;
    nodes.push_back(jumpPtrLoop);
    nodes.insert(nodes.end(), body.begin(), body.end());
    nodes.insert(nodes.end(), cond.begin(), cond.end());
    nodes.push_back(new Jump(jumpPtrLoop));
    nodes.push_back(jumpPtrEnd);

    return nodes;
}

vector<RTLNode*> ForStmnt::toRTL()
{
    Flag* jumpPtrLoop = new Flag();
    Flag* jumpPtrEnd = new Flag();

    vector<RTLNode*> cond = this->cond->toRTL(jumpPtrEnd);
    vector<RTLNode*> body = this->stmnts->toRTL();
    vector<RTLNode*> after = this->after->toRTL();

    vector<RTLNode*> nodes = this->init->toRTL();
    nodes.push_back(jumpPtrLoop);
    nodes.insert(nodes.end(), cond.begin(), cond.end());
    nodes.insert(nodes.end(), body.begin(), body.end());
    nodes.insert(nodes.end(), after.begin(), after.end());
    nodes.push_back(new Jump(jumpPtrLoop));
    nodes.push_back(jumpPtrEnd);

    return nodes;
}


bool ForStmnt::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}
bool DoStmnt::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}
bool WhileStmnt::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}

bool IfStmnt::checkVariables(vector<RTLObject *> *variables, vector<RTLObject *> iterators){
    
}