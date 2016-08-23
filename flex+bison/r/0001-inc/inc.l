%option noyywrap

%x INCFILE

%{
struct file_desc {
    struct file_desc *prev;
    YY_BUFFER_STATE bs;
    FILE *fp;
};

struct file_desc *cur_desc;

%}

%%
^"#"[ \t]*"include"[ \t]*[<\"][ \t]*  { BEGIN INCFILE; }

<INCFILE>[^ \t\">]* {
    { int c; while((c = input()) && c != '\n'); }   // #include<filename --> clear the line tail
    if(newfile(yytext) < 0) yyterminate();
    BEGIN INITIAL;
}

<<EOF>>     { if(popfile() != 0) yyterminate(); }

.       { ECHO; }

%%

void yyerror(char *msg)
{
    printf(msg);
}

int newfile(char *fn)
{
    struct file_desc *p;

    p = malloc(sizeof(struct file_desc));
    if(!p) {
        yyerror("malloc failed.\n");
        return -1;
    }

    p->prev = cur_desc;
    p->fp = fopen(fn, "r");
    if(!p->fp) {
        yyerror("fopen failed.");
        return -1;
    }

    p->bs = yy_create_buffer(p->fp, YY_BUF_SIZE);

    yy_switch_to_buffer(p->bs);

    cur_desc = p;

    return 0;

}

int popfile()
{
    int rc;
    struct file_desc *p;

    if(!cur_desc)
        return -1;

    p = cur_desc;

    fclose(p->fp);
    yy_delete_buffer(p->bs);

    if(p->prev) {
        cur_desc = p->prev;
        if(cur_desc->bs)
            yy_switch_to_buffer(cur_desc->bs);
        rc = 0;
    } else {
        cur_desc = NULL;
        rc = 1;
    }
    free(p);

    return rc;
}

int main(void)
{
    if(newfile("1.inc") == 0)
        yylex();

    return 0;
}


