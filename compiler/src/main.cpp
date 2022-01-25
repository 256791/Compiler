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
    if(yyparse()){
        cout << "\033[31mCompilation terminated\n";
        return 0;
    }

    ast->printXML(0);
    cout << "\n\n";

    RTLProgram *rtl = ast->toRTL();

    cout << "INITIAL\n";
    rtl->printRTL();
    cout << "\n\n";

    rtl->resolveAddresses();

    cout << "RESOLVE ADR\n";
    rtl->printRTL();
    cout << "\n\n";

    rtl->expandVariables(false);

    cout << "EXPAND\n";
    rtl->printRTL();
    cout << "\n\n";

    rtl->expandVariables(true);

    cout << "EXPAND DEEP\n";
    rtl->printRTL();
    cout << "\n\n";

    rtl->allocateRegisters();

    cout << "ALLOCATE\n";
    rtl->printRTL();
    cout << "\n\n";

    ofstream file;
    file.open("prog.mr");
    for (auto n : rtl->toAll())
    {
        // n->toASM(cout);
        n->toASM(file);
    }
    file << "HALT\n";
    file.close();

    return 0;
}