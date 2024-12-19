/*
*   Funções Auxiliares para uma calculadora avançadaa
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "calc.h"

struct symbol symtab[NHASH];

/* Funções em C para TS */

/* Função Hashing */
static unsigned symhash(char *sym){
    
    unsigned int hash = 0;
    unsigned c;

    while(c = *sym++)
        hash = hash * 9 ^ c;

    return hash;

}

struct symbol *lookup(char *sym){

    struct symbol *sp = &symtab[symhash(sym) % NHASH];
    int scount = NHASH;

    while(--scount >= 0){

        if(sp->name && !strcasecmp(sp->name, sym))
            return sp;

        if(!sp->name){ /* Nova entrada na TS */

            sp->name = strdup(sym);
            sp->value = 0;
            sp->func = NULL;
            sp->syms = NULL;
            return sp;

        }

        if(++sp >= symtab + NHASH)
            sp = symtab; /* Tenta a próxima entrda */

    }

    yyerror("Overflow na tabela de símbolos\n");
    abort(); /* Tabela está cheia */

}

struct ast *newast(int nodetype, struct ast *l, struct ast *r){

    struct ast *a = (struct ast *)malloc(sizeof(struct ast));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo AST\n");
        exit(0);
    }

    a->nodetype = nodetype;
    a->l = l;
    a->r = r;

    return a;

}

struct ast *newnum(double d){

    struct numval *a = (struct numval* )malloc(sizeof(struct numval));

    if(!a){
        yyerror("Falha ao alocar struct numval\n");
        exit(0);
    }

    a->nodetype = 'K';
    a->number = d;

    return (struct ast *)a;

}

struct ast *newcmp(int cmptype, struct ast *l, struct ast *r){

    struct ast *a = (struct ast *)malloc(sizeof(struct ast));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo ast\n");
        exit(0);
    }

    a->nodetype = '0' + cmptype;
    a->l = l; 
    a->r = r;

    return a;

}

struct ast *newfunc(int functype, struct ast *l){

    struct fncall *a = (struct fncall *)malloc(sizeof(struct fncall));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo fncall\n");
        exit(0);
    }

    a->nodetype = 'F';
    a->l  = l;
    a->functype = functype;

    return (struct ast *)a;

}

struct ast *newcall(struct symbol *s, struct ast *l){

    struct ufncall *a = (struct ufncall *)malloc(sizeof(struct ufncall));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo ufncall\n");
        exit(0);
    }

    a->nodetype = 'C';
    a->l = l;
    a->s = s;

    return (struct ast *)a;

}

struct ast *newref(struct symbol *s){

    struct symref *a = (struct symref *)malloc(sizeof(struct symref));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo symref\n");
        exit(0);
    }

    a->nodetype = 'N';
    a->s = s;

    return (struct ast *)a;

}

struct ast *newasgn(struct symbol *s, struct ast *v){

    struct symasgn *a = (struct symasgn *)malloc(sizeof(struct symasgn));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo symasgn\n");
        exit(0);
    }

    a->nodetype = '=';
    a->s = s;
    a->v = v;

    return (struct ast *)a;

}

struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *el){

    struct flow *a = (struct flow *)malloc(sizeof(struct flow));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo flow\n");
        exit(0);
    }

    a->nodetype = nodetype;
    a->cond = cond;
    a->tl = tl;
    a->el = el;

    return (struct ast *)a;

}

struct ast *newfor(int nodetype, struct ast *ini, struct ast *cond, struct ast *inc, struct ast *list){

    struct Tfor *a = (struct Tfor *)malloc(sizeof(struct Tfor));

    if(!a){
        yyerror("Falha ao alocar novo nó do tipo Tfor\n");
        exit(0);
    }

    a->nodetype = 'O';
    a->ini = ini;
    a->cond = cond;
    a->inc = inc;
    a->list = list;

    return (struct ast *)a;

}

/* Liberar uma árvore AST */

void treefree(struct ast *a){

    if(!a)
        return;

    switch(a->nodetype){

        /* duas subarvores */
        case '+': 
        case '-':
        case '*':
        case '/':
        case '%':
        case '&':
        case '|':
        case '1': case '2': case '3': case '4': case '5': case '6':
        case 'L':
            treefree(a->r);

        /* uma subarvore */    
        case 'C':
        case 'F':
            treefree(a->l);

        /* sem subarvore */
        case 'K':
        case 'N':
            break;

        case '=':
            free(((struct symasgn *)a)->v);
            break;

        /* Acima de 3 subarvores */

        case 'I':
        case 'W':
            treefree(((struct flow *)a)->cond);
            if(((struct flow *)a)->tl)
                treefree(((struct flow *)a)->tl);
            if(((struct flow *)a)->el)
                treefree(((struct flow *)a)->el);
            break;
        
        case 'O':
            treefree(((struct Tfor *)a)->ini);
            treefree(((struct Tfor *)a)->cond);
            treefree(((struct Tfor *)a)->inc);
            treefree(((struct Tfor *)a)->list);
            break;

        default:
            printf("Erro interno: free bad node %c", a->nodetype);

    }

    free(a); /* Sempre libera o próprio nó */

}

struct symlist *newsymlist(struct symbol *sym, struct symlist *next){

    struct symlist *sl = (struct symlist *)malloc(sizeof(struct symlist));

    if(!sl){
        yyerror("Falha ao alocar novo nó do tipo symlist\n");
        exit(0);
    }

    sl->sym = sym;
    sl->next = next;

    return sl;

}

/* Libera uma lista de símbolos */
void symlistfree(struct symlist *sl){

    struct symlist *nsl;

    while(sl){
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}

/* Etapa principal >> avaliação de expressões, comandos, funções, ... */

static double callbuiltin(struct fncall *);
static double calluser(struct ufncall *);

double eval(struct ast *a){

    double v;

    if(!a){
        yyerror("Erro interno: null eval\n");
        return 0.0;
    }

    switch(a->nodetype){

        /* Constante */
        case 'K':
            v = ((struct numval *)a)->number;
            break;

        /* Referência de nome */
        case 'N':
            v = ((struct symref *)a)->s->value;
            break;

        /* Atribuição */
        case '=':
            v = ((struct symasgn *)a)->s->value = eval(((struct symasgn *)a)->v);
            break;

        /* Expressões */
        case '+': 
            v = eval(a->l) + eval(a->r);
            break;

        case '-': 
            v = eval(a->l) - eval(a->r);
            break;

        case '*': 
            v = eval(a->l) * eval(a->r);
            break;

        case '/': 
            v = eval(a->l) / eval(a->r);
            break;

        case '%':
            v = (double)((int)eval(a->l) % (int)eval(a->r));
            break;

        case '&':
            v = eval(a->l) && eval(a->r);
            break;

        case '|':
            v = eval(a->l) || eval(a->r);
            break;

        /* Comparações */

        case '1':
            v = (eval(a->l) > eval(a->r)) ? 1 : 0;
            break;

        case '2':
            v = (eval(a->l) < eval(a->r)) ? 1 : 0;
            break;

        case '3':
            v = (eval(a->l) != eval(a->r)) ? 1 : 0;
            break;

        case '4':
            v = (eval(a->l) == eval(a->r)) ? 1 : 0;
            break;

        case '5':
            v = (eval(a->l) >= eval(a->r)) ? 1 : 0;
            break;

        case '6':
            v = (eval(a->l) <= eval(a->r)) ? 1 : 0;
            break;
        
        /* Controle de fluxo */
        /* Gramática permite expressões vazias, então dever ser verificadas */

        /* if/then/else */

        case 'I':
            if(eval(((struct flow *)a)->cond) != 0){ /* Verifica condição */

                if(((struct flow *)a)->tl)
                    v = eval(((struct flow *)a)->tl);
                else 
                    v = 0.0; /* Valor dafault */
            } else{

                if(((struct flow *)a)->el) /* Ramo else */
                    v = eval(((struct flow *)a)->el);
                else 
                    v = 0.0; /* Valor dafault */
            }
            
            break;

        case 'W': 
            v = 0.0; /* Valor dafault */

            if(((struct flow *)a)->tl) /* Testando se a lista de comandos não é vazia */
                while(eval(((struct flow *)a)->cond) != 0) /* Avalia a condição */
                    v = eval(((struct flow *)a)->tl);   /* Avalia os comandos*/

            break;      /* Valor do último comando  é o valor do while/do */

        case 'O': // For
            v = 0.0; /* Valor dafault */

            v = eval(((struct Tfor *)a)->ini);

            while(eval(((struct Tfor *)a)->cond) != 0){

                v = eval(((struct Tfor *)a)->list);

                v = eval(((struct Tfor *)a)->inc);

            }

            break; 


        /* Lista de comandos */

        case 'L': 
            eval(a->l);
            v = eval(a->r);
            break;
        
        case 'F': 
            v = callbuiltin((struct fncall *)a);
            break;
        
        case 'C':
            v = calluser((struct ufncall *)a);
            break;

        default: printf("Erro interno: bad node %c\n", a->nodetype);
            
    }

    return v;

}

static double callbuiltin(struct fncall *f){

    enum bifs functype = f->functype;
    double v = eval(f->l);

    switch(functype){

        case B_sqrt: 
            return sqrt(v);
        case B_exp:
            return exp(v);
        case B_log:
            return log(v);
        case B_print:
            printf(" = %4.4g\n", v);
            return v;
        default: 
            yyerror("Função pré-definida %d desconhecida\n", functype);
            return 0.0;
    }
}

/* Função definida pelo usuário */

void dodef(struct symbol *name, struct symlist *syms, struct ast *func){

    if(name->syms)
        symlistfree(name->syms);

    if(name->func)
        treefree(name->func);

    name->syms = syms;
    name->func = func;

}

static double calluser(struct ufncall *f){

    struct symbol *fn = f->s;   /* Nome da função */
    struct symlist *sl;         /* Argumentos (originais) da função */
    struct ast *args = f->l;    /* Argumentos (usados) na função */
    double *oldval, *newval;    /* Salvar valores de argumentos */
    double v;
    int nargs;
    int i;

    if(!fn->func){
        yyerror("Chamada para função %d indefinida\n", fn->name);
        return 0;
    }

    /* Contar argumentos */

    sl = fn->syms;
    for(nargs = 0; sl; sl = sl->next)
        nargs++;

    /* Prepara o para salvar argumentos */
    oldval = (double *)malloc(nargs * sizeof(double));
    newval = (double *)malloc(nargs * sizeof(double));

    if(!oldval  || !newval){
        yyerror("Sem espaço em %s\n", fn->name);
        return 0.0;
    }

    /* Avaliação de argumentos */
    for(i = 0; i < nargs; i++){

        if(!nargs){
            yyerror("Poucos argumentos na chamada da função %s\n", fn->name);
            free(oldval);
            free(newval);
            return 0.0;
        }

        if(args->nodetype == 'L'){       /*Se é uma lista de nós */
            newval[i] = eval(args->l);
            args = args->r;
        } else{                          /*Se é o final da lista */
            newval[i] = eval(args);
            args = NULL;
        }
    }

    /* Salvar valores (originais) dos argumentos, atribuir novos valores */

    sl = fn->syms;


    for(i = 0; i < nargs; i++){

        struct symbol *s = sl->sym;

        oldval[i] = s->value;
        s->value = newval[i];
        sl = sl->next;
    }

    free(newval);

    /* Avaliação da função */
    v = eval(fn->func);

    /* Recolocar os valores (originais) da função */

    sl = fn->syms;

    for(i = 0; i < nargs; i++){

        struct symbol *s = sl->sym;

        s->value = oldval[i];
        sl = sl->next;

    }

    free(oldval);
    return v;

}

void yyerror(char *s, ...){

    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");

}

char *curfilename; 
extern int yyparse();
extern FILE *yyin;


int main(int argc, char **argv) {

    if(argc < 2){ 
        curfilename = "(stdin)";
        yyin = stdin;
        yyparse();
        printf("Leitura finalizada da entrada padrão\n");
        return 0;
    }

    for(int i = 1; i < argc; i++){

        FILE *f = fopen(argv[i], "r");

        if(!f){
            perror(argv[i]);
            return 1;
        }

        curfilename = argv[i]; 
        yyin = f; 

        printf("          Lendo arquivo: %s\n\n", argv[i]);
        yyparse();
        fclose(f);
        printf("\n          Arquivo %s processado.\n\n", argv[i]);

    }
        

    return 0;
}