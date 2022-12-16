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

typedef struct date{
    size_t day;
    size_t month;
    size_t year;
    size_t time;
}Tdate;

typedef struct oldestM{
    Tdate date;
    char used;
    size_t old_count;
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

//returns 1 for weekend or 0 for weekday
size_t dayToNum(char * s);

//returns month number 
size_t monthToNum (char * s);

void insertYearL(queryADT query, TYear * years);

queryADT newQuery(size_t yearFrom, size_t yearTo);
    
void addOldest(queryADT q, size_t ID, Tdate date, size_t pedestrians);

void freeQuery(queryADT q);

void makeSenL(queryADT q);

void q1(queryADT q, FILE * csvQuery, htmlTable tableQuery );

void q2(queryADT q, FILE * csvQuery, htmlTable tableQuery );

void q3(queryADT q, FILE * csvQuery, htmlTable tableQuery );

void q4(queryADT q, FILE * csvQuery, htmlTable tableQuery );

void q5(queryADT q, FILE * csvQuery, htmlTable tableQuery );

#endif