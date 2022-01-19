%{
    #include "ast/AST.h"
    #include "parser.tab.h"
%}

%option noyywrap

%%

[_a-z]+     {yylval.name = new std::string(yytext); return(PIDENTIFIER);}

[\[\]:;,]    {return(yytext[0]);}

-?[0-9]+      {yylval.num = atoi(yytext); return(NUM);}

\(.*\)      {}

[\t\n ]     {}

"VAR"       {return(VAR);}
"BEGIN"     {return(BEG);}
"END"       {return(END);}
"ASSIGN"    {return(ASSIGN);}

"IF"        {return(IF);}
"THEN"      {return(THEN);}
"ELSE"      {return(ELSE);}
"ENDIF"     {return(ENDIF);}

"WHILE"     {return(WHILE);}
"ENDWHILE"  {return(ENDWHILE);}
"FOR"       {return(FOR);}
"ENDFOR"    {return(ENDFOR);}
"REPEAT"    {return(REPEAT);}
"UNTIL"     {return(UNTIL);}
"FROM"      {return(FROM);}
"TO"        {return(TO);}
"DOWNTO"    {return(DOWNTO);}
"DO"        {return(DO);}

"READ"      {return(READ);}
"WRITE"     {return(WRITE);}

"PLUS"      {return(PLUS);}
"MINUS"     {return(MINUS);}
"TIMES"     {return(TIMES);}
"DIV"       {return(DIV);}
"MOD"       {return(MOD);}

"EQ"        {return(EQ);}
"NEQ"       {return(NEQ);}
"LE"        {return(LE);}
"GE"        {return(GE);}
"LEQ"       {return(LEQ);}
"GEQ"       {return(GEQ);}

%%