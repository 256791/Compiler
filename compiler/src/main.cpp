#include <stdio.h>  
#include <typeinfo>
#include "ast/AST.h"
#include "rtl/RTL.h"
#include "parser.tab.h"

extern FILE *yyin;
extern FILE *yyout;
extern int yyparse(void);
extern AST *ast;

int main()
{
    yyparse();
    // ast->printXML(0);
    // cout << endl << endl;
    Program* program = ast->toRTL();

    program->printRTL();
        
    // program->resolveAddresses();
    // program->expandVariables();

    // program->printRTL();
    // cout << endl << endl;

    // program->allocateRegisters();
    // program->printRTL();
}