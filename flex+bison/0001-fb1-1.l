/* unix wc */
%{
    int chars = 0;
    int words = 0;
    int lines = 0;
%}

%option noyywrap

%%
[a-zA-Z]+   { words++; chars += strlen(yytext); }
\n          { chars++; lines++; }
.           { chars++; }
%%

int main(void)
{

    yyin = fopen("file.in", "r");
    if(!yyin) {
        perror("fopen failed.\n");
        return -1;
    }
    yylex();
    printf("lines: %d, words: %d, chars: %d\n", lines, words, chars);

    return 0;
}

