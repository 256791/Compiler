#include "AST.h"

const string AST::NAME = "Program";

AST::AST(CompoundStmnt *stmnts)
{
    this->stmnts = stmnts;
}

AST::AST(CompoundStmnt *decls, CompoundStmnt *stmnts)
{
    this->decls = decls;
    this->stmnts = stmnts;
}

RTLProgram *AST::toRTL()
{
    vector<RTLObject *> data;
    vector<RTLNode *> commands;

    for (Stmnt *stmnt : this->decls->stmnts)
    {
        for (RTLNode *node : stmnt->toRTL())
        {
            data.push_back(dynamic_cast<RTLObject *>(node));
        }
    }

    for (Stmnt *stmnt : this->stmnts->stmnts)
    {
        vector<RTLNode *> to_append = stmnt->toRTL();
        commands.insert(commands.end(), to_append.begin(), to_append.end());
    }
    

    return new RTLProgram(data, commands);
};