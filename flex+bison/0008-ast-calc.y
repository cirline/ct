%{
#include <stdio.h>
#include <stdlib.h>

struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct numval {
    int nodetype;
    double d;
};

struct ast * newnum(double d);
double calc(struct ast *tree);

%}

%union {
struct ast * a;
double d;
}

%token <d> NUMBER
%token EOL

%type <a> exp /*factor term*/

%%
calclist: /* nothing */
        | calclist exp EOL {
            printf("= %g\n", calc($2));
            printf("\n> ");
        }
        | calclist EOL { printf("> "); }
        ;
exp: NUMBER { $$ = newnum($1); }
   ;
%%

int main(void)
{
    printf("> ");
    yyparse();
    return 0;
}

yyerror(char *s)
{
    printf("yyerror: %s", s);
    abort();
}

struct ast * newnum(double d)
{
    struct numval * np;

    np = malloc(sizeof(struct numval));
    if(!np) {
        perror("malloc");
        abort();
    }

    np->nodetype = 'K';
    np->d = d;
    
    return (struct ast *)np;
}

double calc(struct ast *tree)
{
    if(!tree)
        return 0;

    switch(tree->nodetype) {
    case 'K':
        return ((struct numval *)tree)->d;
        break;
    default:
        yyerror("node type fault.\n");
        abort();
    }
}
