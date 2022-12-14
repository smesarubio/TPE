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

queryADT newQuery(size_t yearFrom, size_t yearTo){
    queryADT q = calloc(1, sizeof(queryCDT));
    if (q == NULL){
        perror("Unable to allocate memory.");
        exit(1);
    }
    q->yearFrom = yearFrom;
    q->yearTo = yearTo;
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

char dateCmp(size_t year1, size_t year2, size_t month1, size_t day1, size_t month2, size_t day2){
    if(month1 == month2 && day1==day2){
        return 0; // mismo día
    }
    if (month1 < month2){
        return 1; // día uno antes que día dos.
    }
    if (month1 == month2){
        return day1 < day2;
    }
    return -1; // día dos antes que día uno.
}

void addOldest(queryADT q, size_t ID, size_t month, size_t dayN, size_t time, size_t pedestrians){
    int c = dateCmp(q->sensorsID[ID].oldest.month, month, q->sensorsID[ID].oldest.dayN, dayN);
    if(c==-1){
        q->sensorsID[ID].oldest->old_count = pedestrians;
        q->sensorsID[ID].oldest->dayN = dayN;
        q->sensorsID[ID].oldest->month = month;
        printf("peds: %lu\t", q->sensorsID[ID].oldest->old_count);
        printf("dayN: %lu\t", q->sensorsID[ID].oldest->dayN);
        printf("month: %lu\n", q->sensorsID[ID].oldest->month);
    }
    //si c = 1 o c = 0, no cambia el oldest.

}

void insertYearL(queryADT query, TYear * years){
    query->years = years; 
}

static void freeRecYears(TYear * years){
    if(years == NULL){
        return;
    }
    freeRecYears(years->tail);
    free(years);
}

void freeQuery(queryADT q) {
    for (int j=0; j<86; j++){
        free(q->sensorsID[j].name);
    }
    freeRecYears(q->years);
    free(q->sensorsID);
    free(q);    
}

