%code requires
{
    #include <string>
}
%{
    #include <stdio.h>
    #include "ast/AST.h"

    extern void yyterminate();

    extern int yylex(void);
    void yyerror(char *s);

    AST* ast = nullptr;
%}

%union{
    string* name;
    int num;
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
    dec',' PIDENTIFIER                      { $$ = new CompoundStmnt($1,  new VarDecl(*$3)); }
    | dec',' PIDENTIFIER'['NUM':'NUM']'     { $$ = new CompoundStmnt($1, new ArrDecl(*$3, $5, $7)); }
    | PIDENTIFIER                           { $$ = new CompoundStmnt(new VarDecl(*$1)); }
    | PIDENTIFIER'['NUM':'NUM']'            { $$ = new CompoundStmnt(new ArrDecl(*$1, $3, $5)); }

stmnts:
    stmnts stmnt    { $$ = new CompoundStmnt($1, $2); }
    | stmnt         { $$ = new CompoundStmnt($1); }

stmnt:
    identifier ASSIGN expr';'                                   { $$ = new BinOpExpr('=', $1, $3); }
    | IF cond THEN stmnts ELSE stmnts ENDIF                     { $$ = new IfStmnt($2, $4, $6); }
    | IF cond THEN stmnts ENDIF                                 { $$ = new IfStmnt($2, $4); }
    | WHILE cond DO stmnts ENDWHILE                             { $$ = new WhileStmnt($2, $4); }
    | REPEAT stmnts UNTIL cond';'                               { $$ = new DoStmnt($4, $2); }
    | FOR PIDENTIFIER FROM value TO value DO stmnts ENDFOR      { $$ = new ForStmnt(*$2, $4, $6, 'U', $8); }
    | FOR PIDENTIFIER FROM value DOWNTO value DO stmnts ENDFOR  { $$ = new ForStmnt(*$2, $4, $6, 'D', $8); }
    | READ identifier';'                                        { $$ = new Syscall($2, 'R'); }
    | WRITE value';'                                            { $$ = new Syscall($2, 'W'); }

expr:
    value                   { $$ = $1; }
    | value PLUS value      { $$ = new BinOpExpr('+', $1, $3); }
    | value MINUS value     { $$ = new BinOpExpr('-', $1, $3); }
    | value TIMES value     { $$ = new BinOpExpr('*', $1, $3); }
    | value DIV value       { $$ = new BinOpExpr('/', $1, $3); }
    | value MOD value       { $$ = new BinOpExpr('%', $1, $3); }

cond:
    value EQ value      { $$ = new Comp("==", $1, $3); }
    | value NEQ value   { $$ = new Comp("!=", $1, $3); }
    | value LE value    { $$ = new Comp("<", $1, $3); }  
    | value GE value    { $$ = new Comp(">", $1, $3); }
    | value LEQ value   { $$ = new Comp("<=", $1, $3); }
    | value GEQ value   { $$ = new Comp(">=", $1, $3); }

value:              
    NUM             { $$ = new VarConst($1); }
    | identifier    { $$ = $1; }

identifier:
    PIDENTIFIER                     { $$ = new VarRef(*$1); }
    | PIDENTIFIER'['PIDENTIFIER']'  { $$ = new ArrRef(*$1, new VarRef(*$3)); }
    | PIDENTIFIER'['NUM']'          { $$ = new ArrRef(*$1, new VarConst($3)); }

%%

void yyerror(char* s)
{
    printf("\nError: %s\n",s);
}
