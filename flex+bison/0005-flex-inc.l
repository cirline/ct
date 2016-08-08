
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
int popfile();
%}

%%
^"#"[ \t]*"include"[ \t]*[\"<]      { BEGIN IFILE; }
<IFILE>[^ \t\n\">]+                 {
    {int c; while((c = input()) && c != '\n'); }
    curbs->lineno++;
    if(newfile(yytext) != 0) yyterminate();
    BEGIN INITIAL;
}
<<EOF>>     {
    if(popfile() != 0) yyterminate();
}

^[ \t]*\n   {
    fprintf(yyout, "%4d:\n", curbs->lineno);
    curbs->lineno++;
}
^.      {
    fprintf(yyout, "%4d: ", curbs->lineno);
    ECHO;
}
.       {
    ECHO;
}
\n      {
    fprintf(yyout, "\n"); 
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

int popfile()
{
    struct bufstack *bs_prev;

    if(!curbs)
        return -1;
    if(curbs->f)
        fclose(curbs->f);
    if(curbs->bs)
        yy_delete_buffer(curbs->bs);
    bs_prev = curbs->prev;
    free(curbs);
    if(!bs_prev)
        return 1;
    curbs = bs_prev;
    if(curbs && curbs->bs)
        yy_switch_to_buffer(curbs->bs);

    return 0;
}

