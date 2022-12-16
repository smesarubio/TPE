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

typedef struct queryCDT * queryADT;

void createYearL (FILE * fReadings, queryADT query);

void createSensorV(FILE * fSensor, queryADT q);

//returns 1 for weekend or 0 for weekday
size_t dayToNum(char * s);

//returns month number 
size_t monthToNum (char * s);

queryADT newQuery(size_t yearFrom, size_t yearTo);
    
void addOldest(queryADT q, size_t ID, Tdate date, size_t pedestrians);

//frees every struct inside queryCDT
void freeQuery(queryADT q);

void makeSenL(queryADT q);

//creates query 1
void q1(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 2
void q2(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 3
void q3(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 4
void q4(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 5
void q5(queryADT q, FILE * csvQuery, htmlTable tableQuery );

#endif