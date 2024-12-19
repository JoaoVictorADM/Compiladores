/*
*   Parser para uma calculadora avançada
*/

%{

#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

%}

%union{
    struct ast *a;
    double d;
    struct symbol *s;       /* Qual símbolo */
    struct symlist *sl;
    int fn;                 /* Qual função */
}

%debug

/* Declaração de tokens */
%token <d> NUMBER
%token <s> NAME
%token <fn> FUNC
%token EOL

%token IF THEN ELSE WHILE DO LET FOR

%left OR 
%left AND
%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/' '%'

%type <a> exp stmt list explist
%type <sl> symlist

%start calclist

%%

stmt: IF exp THEN '{' list '}'                                 { $$ = newflow('I', $2, $5, NULL); }
    | IF exp THEN '{' list '}' ELSE '{' list '}'               { $$ = newflow('I', $2, $5, $9); }
    | WHILE exp DO '{' list '}'                                { $$ = newflow('W', $2, $5, NULL); }
    | FOR '(' explist ';' exp ';' explist ')' '{' list '}'     { $$ = newfor('O', $3, $5, $7, $10); }
    | exp
    ;


list:                { $$ = NULL; }
    | stmt ';' list  { if( $3 == NULL) 
                           $$ = $1;
                       else
                           $$ = newast('L', $1, $3);
                     }
    ;

exp: exp CMP exp            { $$ = newcmp($2, $1, $3); }
    | exp '+' exp           { $$ = newast('+', $1, $3); }
    | exp '-' exp           { $$ = newast('-', $1, $3); }
    | exp '*' exp           { $$ = newast('*', $1, $3); }
    | exp '/' exp           { $$ = newast('/', $1, $3); }
    | exp '%' exp           { $$ = newast('%', $1, $3); }
    | exp AND exp           { $$ = newast('&', $1, $3); }
    | exp OR exp            { $$ = newast('|', $1, $3); }
    | '(' exp ')'           { $$ = $2; }
    | NUMBER                { $$ = newnum($1); }
    | NAME                  { $$ = newref($1); }
    | NAME '=' exp          { $$ = newasgn($1, $3); }
    | FUNC '(' explist ')'  { $$ = newfunc($1, $3); }
    | NAME '(' explist ')'  { $$ = newcall($1, $3); }
    ;

explist: exp
       | exp ',' explist    { $$ = newast('L', $1, $3); }
       ;

symlist: NAME               { $$ = newsymlist($1, NULL); }
       | NAME ',' symlist   { $$ = newsymlist($1, $3); }
       ;

calclist: 
        | calclist stmt EOL {
                                printf("= %4.4g\n", eval($2));
                                treefree($2);
                            }
        | calclist LET NAME '(' symlist ')' '=' '{' list '}' EOL {
                                                            dodef($3, $5, $9);
                                                            printf("Defined %s\n", $3->name);
                                                         }
        | calclist error EOL { yyerrok; printf("> "); }
        ;

