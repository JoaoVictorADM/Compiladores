/*
*   Declarações para um tradutor
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define NCHILDREN 62

/* Interface com o lexer */
extern int yylineno;
void yyerror(char *s, ...);

typedef struct String{

    char *data;
    struct String *next;

}TString;

typedef struct Plan{

    char *trigger;
    char *ctx;
    TString *body;
    struct Plan *next;

}Plan;

typedef struct Agent{

    char *name;
    TString *beliefs;
    char *goal;
    Plan *plans;
    struct Agent *next;

}Agent;

TString *createTStringNode(char *data);
TString *insertTString(char *data, TString *root);
bool searchTString(char *data, TString *root);
void freeTStringList(TString *root);
Plan *createPlan(char *trigger, char *ctx, TString *body);
Plan *insertPlan(Plan *newPlan, Plan *root);
void freePlanList(Plan *root);
Agent *createAgent(char *name, TString *beliefs, char *goal, Plan *plans);
Agent *insertAgent(Agent *newAgent, Agent *root);
void freeAgentList(Agent *root);
char* determineTriggerCtx(char* data, TString *beliefs, TString *goals);
void generatePythonCode(Agent *root);
void yyerror(char *s, ...);