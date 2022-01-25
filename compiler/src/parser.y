%code requires
{
    #include <string>
}
%{
    #include <stdio.h>
    #include <cstring>
    #include "ast/AST.h"

    extern void yyterminate();
    extern int yylineno;

    extern int yylex(void);
    void yyerror(char *s);

    AST* ast = nullptr;
%}

%locations

%union{
    string* name;
    long long num;
    Stmnt* node;
    CompoundStmnt* nodes;
}

%token <num> NUM
%token <name> PIDENTIFIER
%token VAR BEG END ASSIGN
%token IF THEN ELSE ENDIF
%token WHILE ENDWHILE FOR ENDFOR REPEAT UNTIL FROM TO DOWNTO DO
%token READ WRITE
%token PLUS MINUS TIMES DIV MOD
%token EQ NEQ LE GE LEQ GEQ

%type <node> prog
%type <node> value 
%type <node> expr
%type <node> identifier
%type <node> stmnt
%type <node> cond
%type <nodes> dec
%type <nodes> stmnts


%%
prog:
    VAR dec BEG stmnts END    { ast = new AST($2, $4); }
    | BEG stmnts END          { ast = new AST($2); }

dec:
    dec',' PIDENTIFIER                      { $$ = new CompoundStmnt(yylineno, $1,  new VarDecl(yylineno, *$3)); }
    | dec',' PIDENTIFIER'['NUM':'NUM']'     {
            if($5>$7){
                char buffer [100];
                sprintf(buffer, "wrong array definition %i > %i at line %i", $5, $7, yylineno);
                yyerror(buffer);
                YYERROR;
            }
            $$ = new CompoundStmnt(yylineno, $1, new ArrDecl(yylineno, *$3, $5, $7));
        }
    | PIDENTIFIER                           { $$ = new CompoundStmnt(yylineno, new VarDecl(yylineno, *$1)); }
    | PIDENTIFIER'['NUM':'NUM']'            { $$ = new CompoundStmnt(yylineno, new ArrDecl(yylineno, *$1, $3, $5)); }

stmnts:
    stmnts stmnt    { $$ = new CompoundStmnt(yylineno, $1, $2); }
    | stmnt         { $$ = new CompoundStmnt(yylineno, $1); }

stmnt:
    identifier ASSIGN expr';'                                   { $$ = new BinOpExpr(yylineno, '=', $1, $3); }
    | IF cond THEN stmnts ELSE stmnts ENDIF                     { $$ = new IfStmnt(yylineno, $2, $4, $6); }
    | IF cond THEN stmnts ENDIF                                 { $$ = new IfStmnt(yylineno, $2, $4); }
    | WHILE cond DO stmnts ENDWHILE                             { $$ = new WhileStmnt(yylineno, $2, $4); }
    | REPEAT stmnts UNTIL cond';'                               { $$ = new DoStmnt(yylineno, $4, $2); }
    | FOR PIDENTIFIER FROM value TO value DO stmnts ENDFOR      { $$ = new ForStmnt(yylineno, *$2, $4, $6, 'U', $8); }
    | FOR PIDENTIFIER FROM value DOWNTO value DO stmnts ENDFOR  { $$ = new ForStmnt(yylineno, *$2, $4, $6, 'D', $8); }
    | READ identifier';'                                        { $$ = new Syscall(yylineno, $2, 'R'); }
    | WRITE value';'                                            { $$ = new Syscall(yylineno, $2, 'W'); }

expr:
    value                   { $$ = $1; }
    | value PLUS value      { $$ = new BinOpExpr(yylineno, '+', $1, $3); }
    | value MINUS value     { $$ = new BinOpExpr(yylineno, '-', $1, $3); }
    | value TIMES value     { $$ = new BinOpExpr(yylineno, '*', $1, $3); }
    | value DIV value       { $$ = new BinOpExpr(yylineno, '/', $1, $3); }
    | value MOD value       { $$ = new BinOpExpr(yylineno, '%', $1, $3); }

cond:
    value EQ value      { $$ = new Comp(yylineno, "==", $1, $3); }
    | value NEQ value   { $$ = new Comp(yylineno, "!=", $1, $3); }
    | value LE value    { $$ = new Comp(yylineno, "<", $1, $3); }  
    | value GE value    { $$ = new Comp(yylineno, ">", $1, $3); }
    | value LEQ value   { $$ = new Comp(yylineno, "<=", $1, $3); }
    | value GEQ value   { $$ = new Comp(yylineno, ">=", $1, $3); }

value:              
    NUM             { $$ = new VarConst(yylineno, $1); }
    | identifier    { $$ = $1; }

identifier:
    PIDENTIFIER                     { $$ = new VarRef(yylineno, *$1); }
    | PIDENTIFIER'['PIDENTIFIER']'  { $$ = new ArrRef(yylineno, *$1, new VarRef(yylineno, *$3)); }
    | PIDENTIFIER'['NUM']'          { $$ = new ArrRef(yylineno, *$1, new VarConst(yylineno, $3)); }

%%

void yyerror(char* s)
{
    if(strcmp("Syntax error", s) == 0){
        printf("\n\033[31mError\033[0m at line %i %s\n", yylineno, s);
    }
    else if(strcmp("Not enough space for parser stacks", s) == 0){
        printf("\n\033[31mError %s\033[0m\n", s);
    }else if(strcmp("Parser stack overflow", s) == 0){
        printf("\n\033[31mError %s\033[0m\n", s);
    }else{
        printf("\n\033[31mError\033[0m %s at line %i\n", s, yylineno);
    }
}
