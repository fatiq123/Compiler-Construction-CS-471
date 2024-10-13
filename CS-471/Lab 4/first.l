%{
#include <stdio.h>
%}

%% 
[ \t\n]+        ;   // Ignore whitespace
[0-9]+          { printf("NUMBER: %s\n", yytext); }
"+"             { printf("PLUS: %s\n", yytext); }
"-"             { printf("MINUS: %s\n", yytext); }
"*"             { printf("MULTIPLY: %s\n", yytext); }
"/"             { printf("DIVIDE: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("IDENTIFIER: %s\n", yytext); }
.               { printf("UNKNOWN TOKEN: %s\n", yytext); }
%%

int main() {
    printf("\nEnter the string here:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}