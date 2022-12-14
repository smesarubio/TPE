#include <stdio.h>
#include <stdlib.h>
#include "queryTAD.h"

typedef struct queryCDT{
    size_t yearFrom;
    size_t yearTo; 
    TYear * years; //lista por aÃ±o
    TSensor * sensorsID;  //sensores ordenados por ID
    TNodeS * sensorsP; //sensores ordenados por peatones
}queryCDT;

queryADT newQuery(){
    queryADT q = calloc(1, sizeof(queryCDT));
    if (q == NULL){
        perror("Unable to allocate memory.");
        exit(1);
    }
    return q;
}

void insertVector(queryADT q, TSensor * vec){
    q->sensorsID = vec;
    /*for (int i=0; i<86; i++){
        printf("ID: %i\t", i + 1);
        printf("NAME: %s\t", q->sensorsID[i].name);
        printf("FLAG: %c\n", q->sensorsID[i].flag);
    }*/
}

void freeQuery(queryADT q) {
    for (int j=0; j<86; j++){
        free(q->sensorsID[j].name);
    }
    free(q->sensorsID);
    free(q);    
}

