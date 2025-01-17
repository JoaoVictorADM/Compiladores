%{

#include <stdio.h>
#include <stdlib.h>
#include "tradutor.h"

%}

%union{

    Agent *a;
    Plan *p;
    TString *tS;
    char *s;

}

%token <s> NAME NAMEBODY
%token DP AGENTCODE BELIEFS PLANS TRIGGER CTX BODY GOAL EOL

%type <a> json agent agentList
%type <p> plan plans planList
%type <s> trigger ctx goal goalName agentName
%type <tS> body bodyList beliefs beliefsList

%start json

%%

json: '{' '"'AGENTCODE'"' DP agentList '}'              { $$ = $6; generatePythonCode($6); }
    ;

agentList: agent                                        { $$ = $1; }
         | agent agentList                              { $$ = insertAgent($1, $2); }
         ;

agent: '{' agentName',' beliefs',' goal',' plans '}'    { $$ = createAgent($2, $4, $6, $8); }
     ;

agentName: '"'NAME'"' DP '"'NAME'"'                     { $$ = $6; }
         ;




beliefs: '"'BELIEFS'"' DP '[' ']'                       { $$ = NULL; }
       | '"'BELIEFS'"' DP '[' beliefsList ']'           { $$ = $6; }
       ;

beliefsList: '"'NAME'"'                                 { $$ = insertTString($2, NULL); }
           | '"'NAME'"' ',' beliefsList                 { $$ = insertTString($2, $5); }
           ;




goal: '"'GOAL'"' DP goalName                            { $$ = $5; }
    ;

goalName: '"''"'                                        { $$ = strdup(" "); }
        | '"'NAME'"'                                    { $$ = $2; }
        ;   




plans: '"'PLANS'"' DP '{' planList '}'                  { $$ = $6; }
     ;

planList:                                               { $$ = NULL; }
        | plan planList                                 { $$ = insertPlan($1, $2); }
        ;   

plan: '"'NAME'"' DP '{' trigger',' body '}'             { $$ = createPlan($6, NULL, $8); }
    | '"'NAME'"' DP '{' trigger',' ctx',' body '}'      { $$ = createPlan($6, $8, $10); }
    ;




trigger: '"'TRIGGER'"' DP '"'NAME'"'                    { $$ = $6; }
       ;

ctx: '"'CTX'"' DP '"'NAME'"'                            { $$ = $6; }
   ;




body: '"'BODY'"' DP '[' ']'                             { $$ = NULL; }
    | '"'BODY'"' DP '[' bodyList ']'                    { $$ = $6; }
    ;

bodyList: '"'NAMEBODY'"'                                { $$ = insertTString($2, NULL); }
        | '"'NAMEBODY'"' ',' bodyList                   { $$ = insertTString($2, $5); }
        ;
