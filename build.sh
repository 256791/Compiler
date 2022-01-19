#!/bin/bash
bison -d -o "compiler/src/parser.tab.c" "compiler/src/parser.y"
flex -o "compiler/src/lexer.yy.c" "compiler/src/lexer.l"
g++ -w -o "compiler/compiler" \
    "compiler/src/main.cpp" "compiler/src/parser.tab.c" "compiler/src/lexer.yy.c" \
    "compiler/src/ast/AST.cpp" "compiler/src/ast/Stmnt.cpp" "compiler/src/ast/Var.cpp" "compiler/src/ast/Branch.cpp" "compiler/src/ast/Expr.cpp" \
    "compiler/src/ast/ASTXMLPrinter.cpp" \
    "compiler/src/rtl/RTL.cpp" "compiler/src/rtl/RTLPrinter.cpp" "compiler/src/rtl/RegisterAllocator.cpp"