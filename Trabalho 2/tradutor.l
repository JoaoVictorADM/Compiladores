%option noyywrap nodefault yylineno

%{

#include "tradutor.h"
#include "tradutor.tab.h"

%}

name         [a-zA-Z][a-zA-Z0-9]*
namebody     [AGB]"_"{name} 


%%

"{" | /* Operadores de caractere único */
"}" |
"]" |
"[" |
"," |
\"           { return yytext[0]; }        

":"          { return DP; }
"agentCode"  { return AGENTCODE; }
"beliefs"    { return BELIEFS; }
"goal"       { return GOAL; }
"plans"      { return PLANS; }
"body"       { return BODY; }
"trigger"    { return TRIGGER; }
"ctx"        { return CTX; }
{namebody}   { yylval.s = strdup(yytext); return NAMEBODY; }
{name}       { yylval.s = strdup(yytext); return NAME; }

"//".*
[ \t]  /* Igora comentários */

\\\n    { }  /* Ignora continuação de linha */

\n      { return EOL; }

.       { yyerror("Caractere desconhecido %c\n", *yytext); }
%%