/*
*   Funções Auxiliares para uma calculadora avançadaa
*/

#include "tradutor.h"


extern int yyparse();

int main(void){

    return yyparse();

}

TString *createTStringNode(char *data){

    TString *newNode = (TString *)malloc(sizeof(TString));

    if(!newNode){
        printf("Erro ao alocar nó do tipo TString\n");
        exit(1);
    }

    newNode->data = strdup(data);
    newNode->next = NULL;

    return newNode;

}

TString *insertTString(char *data, TString *root){

    TString *newNode = createTStringNode(data);

    if(!root)
        return newNode;

    newNode->next = root;

    return newNode;

}

bool searchTString(char *data, TString *root){

    while(root){

        if(!strcmp(data, root->data))
            return true;

        root = root->next;

    }

    return false;

}

void freeTStringList(TString *root){

    while(root){

        TString *aux = root->next;
        free(root);
        root = aux;

    }

}

Plan *createPlan(char *trigger, char *ctx, TString *body){

    Plan *newNode = (Plan *)malloc(sizeof(Plan));

    if(!newNode){
        printf("Erro ao alocar nó do tipo Plan\n");
        exit(1);
    }

    newNode->trigger = strdup(trigger);
    newNode->ctx = strdup(ctx);
    newNode->body = body;
    newNode->next = NULL;

    return newNode;

}

Plan *insertPlan(Plan *newPlan, Plan *root){

    if(!newPlan){
        printf("Função insertPlan - NewPlan é nulo");
        exit(1);
    }

    newPlan->next = root;

    return newPlan;

}

void freePlanList(Plan *root){

    while(root){

        Plan *aux = root->next;
        freeTStringList(root->body);
        free(root);
        root = aux;

    }

}

Agent *createAgent(char *name, TString *beliefs, char *goal, Plan *plans){

    Agent *newNode = (Agent *)malloc(sizeof(Agent));

    if(!newNode){
        printf("Erro ao alocar nó do tipo Plan\n");
        exit(1);
    }

    newNode->name = name;
    newNode->beliefs = beliefs;
    newNode->goal = goal;
    newNode->plans = plans;
    newNode->next = NULL;

    return newNode;

}

Agent *insertAgent(Agent *newAgent, Agent *root){

    if(!newAgent){
        printf("Função insertAgent - newAgent é nulo");
        exit(1);
    }

    newAgent->next = root;

    return newAgent;

}

void freeAgentList(Agent *root){

    while(root){

        Agent *aux = root->next;
        freeTStringList(root->beliefs);
        freePlanList(root->plans);
        free(root);
        root = aux;

    }

}

char* determineTriggerCtx(char* data, TString *beliefs, TString *goals){
    
    if(searchTString(data, beliefs))
        return strdup("Belief");

    if(searchTString(data, goals))
        return strdup("Goal");    

    printf("%s: Não é nem Belief nem Goal\n", data);
    exit(1);
}

void generatePythonCode(Agent *root){

    if(!root){
        printf("generatePythonCode - Variável root nula\n");
        exit(1);
    }

    FILE *file = fopen("codigo.py", "w");

    if(!file){
        printf("Erro ao abrir o arquivo para escrita\n");
        exit(1);
    }

    fprintf(file, "#======================================================================\n");
    fprintf(file, "# Trabalho de Compiladores\n");
    fprintf(file, "# tradutor : json para maspy\n");
    fprintf(file, "#======================================================================\n\n");

    fprintf(file, "from maspy import *\n\n");

    Agent *aux = root;
    unsigned actualAgent = 1;

    while(root){

        fprintf(file, "class Agente%d(Agent):\n", actualAgent);
        fprintf(file, "\tdef __init__(self, agt_name):\n");
        fprintf(file, "\t\tsuper().__init__(agt_name)\n");
        
        TString *beliefs = root->beliefs;

        while(beliefs){

            fprintf(file, "\t\tself.add(Belief(\"%s\"))\n", beliefs->data);
            beliefs = beliefs->next;

        }

        if(strcmp(root->goal, " "))
            fprintf(file, "\t\tself.add(Goal(\"%s\"))\n", root->goal);

        fprintf(file, "\n");
        
        Plan *plans = root->plans;
        TString *goals = insertTString(root->goal, NULL);

        while(plans){

            char *trigger = determineTriggerCtx(plans->trigger, root->beliefs, goals);
            char *ctx;

            fprintf(file, "\t@pl(gain, %s(\"%s\")", trigger, plans->trigger);

            if(strcmp(plans->ctx, " ")){
                ctx = determineTriggerCtx(plans->ctx, root->beliefs, goals);
                fprintf(file, ", %s(\"%s\")", ctx, plans->ctx);
            }

            fprintf(file, ")\n");

            fprintf(file, "\tdef %s_(self, src):\n", plans->trigger);
                
            TString *body = plans->body;
            
            while(body){

                if(body->data[0] == 'A')
                    fprintf(file, "\t\tself.print(\"%s\")\n", body->data + 2);
                

                if(body->data[0] == 'B'){
                    fprintf(file, "\t\tself.add(Belief(\"%s\"))\n", body->data + 2);
                    root->beliefs = insertTString(body->data + 2, root->beliefs);
                }

                if(body->data[0] == 'G'){
                    fprintf(file, "\t\tself.add(Goal(\"%s\"))\n", body->data + 2);
                    goals = insertTString(body->data + 2, goals);
                }

                body = body->next;

            }

            fprintf(file, "\n");
            
            plans = plans->next;

        }

        actualAgent++;
        root = root->next;

    }

    actualAgent = 1;

    while(aux){

        fprintf(file, "Agente%d(\"%s\")\n", actualAgent, aux->name);
        actualAgent++;
        aux = aux->next;

    }

    fprintf(file, "Admin().start_system()");

}

void yyerror(char *s, ...){

    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");

}
