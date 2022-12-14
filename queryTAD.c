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

//returns 1 for weekend or 0 for weekday
size_t dayToNum(char * s){               
    return s[0] == 'S' || s[0] == 's'; 
}

size_t monthToNum (char * s){
    char *months[] = {"January", "February", "March", "April","May", "June", "July", "August", "September", "October", "November", "December"};
    for (int i=0; i<12; i++){
        if (strcasecmp(s, months[i]) == 0){
            return i+1;
        }
    }
    return -1;
}

static char dateCmp(size_t year1, size_t year2, size_t month1, size_t day1, size_t month2, size_t day2, char * usedFlag,size_t time1 , size_t time2){
    if(*usedFlag==0){
        *usedFlag = 1;
        return -1;
    }
    if (year1 < year2){
        return 1;
    }
    if(month1 == month2 && day1==day2 && year1==year2 && time1 == time2){
        return 0; // mismo día
    }
    if (month1 < month2){
        return 1; // día uno antes que día dos.
    }
    if(month1 == month2 && day1 == day2){
        return time1<time2;
    }
    if (month1 == month2 ){
        return day1 < day2;
    }
    return -1; // día dos antes que día uno.
}

void addOldest(queryADT q, size_t ID, size_t month, size_t dayN, size_t time, size_t pedestrians, size_t year){
    int c = dateCmp(q->sensorsID[ID - 1].oldest.year, year,q->sensorsID[ID - 1].oldest.month, month, q->sensorsID[ID - 1].oldest.dayN, dayN, &(q->sensorsID[ID-1].oldest.used),q->sensorsID[ID - 1].oldest.time, time);
    // printf("%i\n", c);
    if(c==-1){
        q->sensorsID[ID-1].oldest.year = year;
        q->sensorsID[ID-1].oldest.old_count = pedestrians;
        q->sensorsID[ID-1].oldest.dayN = dayN;
        q->sensorsID[ID-1].oldest.month = month;
        q->sensorsID[ID-1].oldest.time = time;
        /*printf("peds: %10lu|\t", q->sensorsID[ID-1].oldest.old_count);
        printf("dayN: %10lu|\t", q->sensorsID[ID-1].oldest.dayN);
        printf("time: %10lu|\t", q->sensorsID[ID-1].oldest.time);
        printf("year: %10lu|\t", q->sensorsID[ID-1].oldest.year);
        printf("month: %lu\n", q->sensorsID[ID-1].oldest.month);*/
    }

    
    //si c = 1 o c = 0, no cambia el oldest.
}

void insertYearL(queryADT query, TYear * years){
    query->years = years; 
}


static TNodeS * makeSenLRec(TNodeS * l, TNodeS * aux, TSensor * vecSen, size_t i){
    // printf("%li\t", i);
    if(vecSen[i].name != NULL){
        if(l == NULL || l->pedestrians > aux->pedestrians 
            || (l->pedestrians == aux->pedestrians && strcasecmp(vecSen[l->ID-1].name, vecSen[i].name)>0)){
            aux->tail = l;
            return aux;
        }
        l->tail = makeSenLRec(l->tail, aux, vecSen, i);    
    }
    return l;
}


void makeSenL(queryADT q){
    for(int i = 0; i < MAX; i++){
        TNodeS * aux= malloc(sizeof(TNodeS));
        if(aux == NULL){
            perror("Unable to allocate memory.");
            exit(1);
        }
        aux->ID = i+1;
        aux->pedestrians = q->sensorsID[i].total;
        aux->tail = NULL;
        q->sensorsP = makeSenLRec(q->sensorsP, aux,  q->sensorsID, i);
    }
    // TNodeS * aux = q->sensorsP;
    // while(aux!= NULL){
    //     printf("ID:%lu\t", q->sensorsP->ID);
    //     printf("Name:%lu\t", q->sensorsP->pedestrians);
    //     aux = aux->tail;
    // }
}

static void freeRecYears(TYear * years){
    if(years == NULL){
        return;
    }
    freeRecYears(years->tail);
    free(years);
}


static void freeRecSen(TNodeS * l){
    if(l==NULL){
        return;
    }
    freeRecSen(l->tail);
    free(l);
}

void freeQuery(queryADT q) {
    for (int j=0; j<86; j++){
        free(q->sensorsID[j].name);
    }
    freeRecYears(q->years);
    freeRecSen(q->sensorsP); 
    free(q->sensorsID);
    free(q);    
}

