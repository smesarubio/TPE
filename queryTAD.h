#ifndef QUERY
#define QUERY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct sensor {
    size_t total;
    char * name;
    size_t len;
    char flag;
    char defective;
}TSensor;

typedef struct Nsensor{
    size_t ID;    
    struct Nsesnor * tail;
}TNodeS;

typedef struct year {
    size_t year;
    size_t total; 
    size_t Dweek;
    size_t month;
    //bool day; //1 for weekend, 0 for weekday
    size_t dayN; //day as a number
    size_t time;
    size_t Dweekend;
    struct year * tail;
}TYear;

typedef struct queryCDT * queryADT;

void insertVector(queryADT q, TSensor * vec);

queryADT newQuery();

void freeQuery(queryADT q);

#endif