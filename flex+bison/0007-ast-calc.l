%option noyywrap nodefault yylineno

%{
#include "0008-ast-calc.tab.h"
%}

%%
"+" |
"-" |
"*" |
"/"         { return yytext[0]; }
[0-9]+      { yylval.d = atof(yytext); return NUMBER;}
\n          { return EOL; }
.           { yyerror(". error\n"); }
%%
