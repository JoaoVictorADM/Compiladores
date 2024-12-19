%option noyywrap nodefault yylineno

%{

#include "calc.h"
#include "calc.tab.h"

%}

/* Expoente float */
EXP ([Ee][-+]?[0-9]+)

%%

"+" | /* Operadores de caractere único */
"-" |
"*" |
"/" |
"%" |
"=" |
"," |
";" |
"(" |
"{" |
"}" |
")"    { return yytext[0]; }

">"   { yylval.fn = 1; return CMP; } /* operadores de comparacao, todos sao token CMP */
"<"   { yylval.fn = 2; return CMP; }
"<>"  { yylval.fn = 3; return CMP; }
"=="  { yylval.fn = 4; return CMP; }
">="  { yylval.fn = 5; return CMP; }
"<="  { yylval.fn = 6; return CMP; }

"if"    { return IF; }      /* Palavras chave */
"then"  { return THEN; }
"else"  { return ELSE; }
"while" { return WHILE; }
"do"    { return DO; }
"let"   { return LET; }
"for"   { return FOR; }
"and"   { return AND; }
"or"    { return OR; }

"sqrt"  { yylval.fn = B_sqrt; return FUNC; }  /* Funções pré-definidas */
"exp"   { yylval.fn = B_exp; return FUNC; }  
"log"   { yylval.fn = B_log; return FUNC; } 
"print" { yylval.fn = B_print; return FUNC; } 

[a-zA-Z][a-zA-Z0-9]*  { yylval.s = lookup(yytext); return NAME; }   /* Nomes */

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?      { yylval.d = atof(yytext); return NUMBER; }

"//".*
[ \t]  /* Igora comentários */

\\\n    { printf("c> "); }  /* Ignora continuação de linha */

\n      { return EOL; }

.       { yyerror("Caractere desconhecido %c\n", *yytext); }
%%