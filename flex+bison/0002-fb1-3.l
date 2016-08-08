%%
"+"     { printf("ADD\n"); }
"-"     { printf("SUB\n"); }
"*"     { printf("MUL\n"); }
"/"     { printf("DIV\n"); }
"|"     { printf("ABS\n"); }
[0-9]+  { printf("NUMBER %s\n", yytext); }
[ \t]   { }
\n      { printf("NEWLINE\n"); }
.       { printf("ERROR\n"); }
%%
