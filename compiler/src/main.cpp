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

int main(int argc, char **argv)
{
    if (argc<2){
        cout << "\033[31mMissing input file\033[0m\n";
        return 0;
    }
    if (argc<3){
        cout << "\033[31mMissing output file\033[0m\n";
        return 0;
    }

    if(!(yyin = fopen(argv[1], "r"))){
        cout << "\033[31mCan't open input file\033[0m\n";
        return 0;
    }

    ofstream file;
    file.open(argv[2]);
    if(!file.is_open()){
        cout << "\033[31mCan't open output file\033[0m\n";
        return 0;
    }

    if (yyparse())
    {
        cout << "\033[31mCompilation terminated\033[0m\n";
        return 0;
    }
    // ast->printXML(0);

    if (!ast->checkVariables())
    {
        cout << "\033[31mCompilation terminated\033[0m\n";
        return 0;
    }

    // ast->printXML(0);

    RTLProgram *rtl = ast->toRTL();

    // rtl->printRTL();


    rtl->resolveAddresses();

    // rtl->printRTL();

    rtl->expandVariables(false);

    rtl->expandVariables(true);

    rtl->allocateRegisters();

    // rtl->printRTL();


    for (auto n : rtl->toAll())
    {
        // n->toASM(cout);
        n->toASM(file);
    }
    file << "HALT\n";
    file.close();

    return 0;
}