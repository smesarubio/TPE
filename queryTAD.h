#ifndef QUERY
#define QUERY
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>

typedef struct sensor {
    size_t total;
    char * name;
    size_t len;
    char flag;
    char defective;
    oldestM oldest;
}TSensor;

typedef struct Nsensor{
    size_t ID;    
    struct Nsesnor * tail;
}TNodeS;

typedef struct oldestM{
    size_t time;
    size_t old_count;
    size_t month;
    size_t dayN; //day as a number
}oldestM;

typedef struct year {
    size_t year;
    size_t total; 
    size_t Dweek;
    size_t Dweekend;
    struct year * tail;
}TYear;

typedef struct queryCDT * queryADT;

void insertVector(queryADT q, TSensor * vec);

queryADT newQuery();

void freeQuery(queryADT q);

#endif