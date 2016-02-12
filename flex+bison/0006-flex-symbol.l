%option noyywrap nodefault yylineno case-insensitive

%{
    struct ref {
        char *filename;
        int lineno;
        int flags;
        struct ref *next;
    };

    struct symbol {
        char *name;
        struct ref *list;
    };

#define HASH_TABLE_LENGTH     9997
    struct symbol symtab[HASH_TABLE_LENGTH];

    struct symbol * lookup(char *name);

    char *current_fn;

    int addref(char *name, int lineno, int flags);
    int prthashtab(void);
%}

%%
[a-z_]+      { addref(yytext, yylineno, 0); }

.|\n        { }
%%

int main(void)
{
    FILE *fp;

    fp = fopen(__FILE__, "r");
    if(!fp) {
        perror("fopen");
        abort();
    }

    yyin = fp;
    yylex();

    prthashtab();

    return 0;
}

unsigned int symhash(char *sym)
{
    unsigned int hash = 0;
    unsigned char c;

    while(c = *sym++)
        hash = hash * 9 ^ c;

    return hash % HASH_TABLE_LENGTH;
}

struct symbol * lookup(char *name)
{
    struct symbol *sp;
    int scnt = HASH_TABLE_LENGTH;

    sp = &symtab[symhash(name)];

    while(--scnt >= 0) {
        if(sp->name && (strcmp(sp->name, name) == 0))
            return sp;
        if(!sp->name) {
            sp->name = strdup(name);
            sp->list = 0;
            return sp;
        }
        if(++sp == symtab + HASH_TABLE_LENGTH) sp = symtab;
    }
    fprintf(yyout, "symbol table is overflow!\n");
    abort();
}

int addref(char *name, int lineno, int flags)
{
//    fprintf(yyout, "%d: %s\n", lineno, name);
    struct symbol *sp;
    struct ref *rp;

    sp = lookup(name);
    rp = malloc(sizeof(struct ref));
    if(!rp) {
        perror("malloc");
        abort();
    }
    rp->lineno = lineno;
    rp->flags = flags;
    rp->next = sp->list;
    sp->list = rp;

    return 0;
}

int prthashtab(void)
{
    struct symbol *sp;
    int pcnt = HASH_TABLE_LENGTH;

    fprintf(yyout, "print hash table:\n");
    while(--pcnt >= 0) {
        sp = &symtab[pcnt];
        if(sp->name) {
            struct ref *rp = sp->list;
            fprintf(yyout, "%s: ", sp->name);
            while(rp) {
                fprintf(yyout, "%d ", rp->lineno);
                rp = rp->next;
            }
            fprintf(yyout, "\n");
        }
    }
    return 0;
}

