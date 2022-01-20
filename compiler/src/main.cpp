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
    ast->printXML(0);
    cout << "\n\n";

    RTLProgram *program = ast->toRTL();

    program->printRTL();
    cout << "\n\n";

    program->resolveAddresses();

    program->printRTL();
    cout << "\n\n";

    program->expandVariables(false);

    program->printRTL();
    cout << "\n\n";

    program->expandVariables(true);

    program->printRTL();
    cout << "\n\n";

    program->allocateRegisters();
    program->printRTL();
}