#include <stdio.h>
#include <typeinfo>
#include "ast/AST.h"
#include "rtl/RTL.h"
#include "rtl/all/ALL.h"
#include "parser.tab.h"

extern FILE *yyin;
extern FILE *yyout;
extern int yyparse(void);
extern AST *ast;

int main()
{
    yyparse();
    // ast->printXML(0);
    // cout << "\n\n";

    RTLProgram *rtl = ast->toRTL();

    rtl->printRTL();
    cout << "\n\n";

    rtl->resolveAddresses();

    // rtl->printRTL();
    // cout << "\n\n";

    rtl->expandVariables(false);

    rtl->printRTL();
    cout << "\n\n";

    rtl->expandVariables(true);

    rtl->printRTL();
    cout << "\n\n";

    rtl->allocateRegisters();

    rtl->printRTL();
    cout << "\n\n";

    ofstream file;
    file.open("prog.imp");
    for (auto n : rtl->toAll())
    {
        // n->toASM(cout);
        n->toASM(file);
    }
    file << "HALT\n";
    file.close();

    return 0;
}