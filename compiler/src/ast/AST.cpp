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

bool AST::checkVariables()
{
    vector<tuple<string, bool, bool>> variables;
    vector<string> empty;
    for (Stmnt *obj : this->decls->stmnts)
    {
        if (ArrDecl *arr = dynamic_cast<ArrDecl *>(obj))
        {
            for (auto v : variables)
            {
                if (get<0>(v) == arr->name)
                {
                    cout << "\n\033[31mError\033[0m Redefinition of variable "
                         << arr->name << " at line " << arr->lineno << "\n";
                    return false;
                }
            }
            variables.push_back(make_tuple(arr->name, true, false));
        }
        else if (VarDecl *var = dynamic_cast<VarDecl *>(obj))
        {
            for (auto v : variables)
            {
                if (get<0>(v) == var->name)
                {
                    cout << "\n\033[31mError\033[0m Redefinition of variable "
                         << var->name << " at line " << var->lineno << "\n";
                    return false;
                }
            }
            variables.push_back(make_tuple(var->name, false, false));
        }
    }
    for (Stmnt *stmnt : this->stmnts->stmnts)
        if (!stmnt->checkVariables(variables, empty))
            return false;

    return true;
}
