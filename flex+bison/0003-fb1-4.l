%{
#include "0004-fb1-5.tab.h"
%}

%%
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"|"     { return ABS; }
[0-9]+  { yylval = atoi(yytext); return NUMBER; }
[ \t]   { }
\n      { return EOL; }
.       { printf("ERROR\n"); }
%%

