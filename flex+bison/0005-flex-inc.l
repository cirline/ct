
%option noyywrap
%x IFILE

%{
    struct bufstack {
        struct bufstack *prev;
        YY_BUFFER_STATE bs;
        int lineno;
        char *filename;
        FILE *f;
    } *curbs = 0;

int newfile(char *filename);
%}

%%
^[ \t]*\n   {
    printf("%4d:\n", curbs->lineno);
    curbs->lineno++;
}
.*      {
    printf("%4d: %s", curbs->lineno, yytext);
}
\n      {
    printf("\n"); 
    curbs->lineno++;
}
%%

int main(void)
{
    if(newfile("file.inc") == 0)
        yylex();
    return 0;
}

int newfile(char *filename)
{
    struct bufstack *bs;

    bs = malloc(sizeof(struct bufstack));
    if(!bs) {
        perror("malloc failed.");
        return -1;
    }
    memset(bs, 0, sizeof(struct bufstack));
    bs->f = fopen(filename, "r");
    if(!bs->f) {
        perror("fopen failed.");
        free(bs);
        return -1;
    }
    bs->bs = yy_create_buffer(bs->f, YY_BUF_SIZE);
    bs->lineno = 1;

    bs->prev = curbs;
    curbs = bs;
    yy_switch_to_buffer(curbs->bs);
    return 0;
}

