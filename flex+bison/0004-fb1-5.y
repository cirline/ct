%{
#include <stdio.h>
%}

%token NUMBER
%token ADD SUB MUL DIV
%token LOP ROP
%token ABS
%token EOL

%%
inputlist: /* nothing */
         | inputlist exp EOL { printf("= %d\n", $2); }
         ;

exp: factor { $$ = $1; }
   | exp ADD factor { $$ = $1 + $3; }
   | exp SUB factor { $$ = $1 - $3; }
   ;

factor: term { $$ = $1; }
      | factor MUL term { $$ = $1 * $3; }
      | factor DIV term { $$ = $1 / $3; }
      ;

term: NUMBER { $$ = $1; }
    | ABS exp ABS { $$ = $2 > 0 ? $2 : -$2; }
    | LOP exp ROP { $$ = $2; }
    ;
%%

int main(void)
{
    yyparse();
}

yyerror(char *s)
{
    fprintf(stderr, "ocurr error: %s\n", s);
}

