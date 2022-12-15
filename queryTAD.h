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
#define MAX 86

typedef struct oldestM{
    size_t year;
    char used;
    size_t time;
    size_t old_count;
    size_t month;
    size_t dayN; //day as a number
}oldestM;

typedef struct sensor {
    size_t total;
    char * name;
    size_t len;
    char flag;
    char defective;
}TSensor;

typedef struct Nsensor{
    size_t ID;    
    size_t pedestrians;
    struct Nsensor * tail;
}TNodeS;

typedef struct year {
    size_t year;
    size_t total; 
    size_t Dweek;
    size_t Dweekend;
    struct year * tail;
}TYear;

typedef struct queryCDT * queryADT;

void insertVector(queryADT q, TSensor * vec);

size_t dayToNum(char * s);

size_t monthToNum (char * s);

void insertYearL(queryADT query, TYear * years);

queryADT newQuery(size_t yearFrom, size_t yearTo);
    
void addOldest(queryADT q, size_t ID, size_t month, size_t dayN, size_t time, size_t pedestrians, size_t year);

void freeQuery(queryADT q);

void makeSenL(queryADT q);

#endif

