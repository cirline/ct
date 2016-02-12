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

struct ast * newast(int type, struct ast * l, struct ast * r);
struct ast * newnum(double d);
double calc(struct ast *tree);

%}

%union {
struct ast * a;
double d;
}

%token <d> NUMBER
%token EOL

%type <a> exp factor term

%%
calclist: /* nothing */
        | calclist exp EOL {
            printf("= %g\n", calc($2));
            printf("\n> ");
        }
        | calclist EOL { printf("> "); }
        ;
exp: factor
   | exp '+' factor     { $$ = newast('+', $1, $3); }
   | exp '-' factor     { $$ = newast('-', $1, $3); }
   ;

factor: term
      | factor '*' term { $$ = newast('*', $1, $3); }
      | factor '/' term { $$ = newast('/', $1, $3); }
      ;

term: NUMBER { $$ = newnum($1); }
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

struct ast * newast(int type, struct ast * l, struct ast * r)
{
    struct ast * tp;

    tp = malloc(sizeof(struct ast));
    if(!tp) {
        perror("malloc");
        abort();
    }

    tp->nodetype = type;
    tp->l = l;
    tp->r = r;

    return tp;
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
    double result = 0;
    if(!tree)
        return result;

    switch(tree->nodetype) {
    case 'K':
        result = ((struct numval *)tree)->d;
        break;
    case '+':
        result = calc(tree->l) + calc(tree->r);
        break;
    case '-':
        result = calc(tree->l) - calc(tree->r);
        break;
    case '*':
        result = calc(tree->l) * calc(tree->r);
        break;
    case '/':
        result = calc(tree->l) / calc(tree->r);
        break;
    default:
        yyerror("node type fault.\n");
        abort();
    }

    return result;
}
