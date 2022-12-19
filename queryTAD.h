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

extern int errno ;

typedef struct date{
    size_t day;
    size_t month;
    size_t year;
    size_t time;
}Tdate;


typedef struct queryCDT * queryADT;

void printVec(queryADT q);

//returns a new queryADT.
queryADT newQuery(size_t yearFrom, size_t yearTo, int * error);

void addSensor(size_t ID, char * name, char * value, queryADT q, int * flag, int * error);

void addYear(queryADT query, size_t count, Tdate date, size_t time, size_t day, size_t ID, int * error);

//creates a list of sensors sorted by pedestrians.
void makeSenL(queryADT q);

//adds the oldest count and date for each sensor and checks for defective sensors.     
void addOldest(queryADT q, size_t ID, Tdate date, size_t pedestrians, size_t time);

//returns 1 for weekend or 0 for weekday.
size_t dayToNum(char * s);

//returns month number.
size_t monthToNum (char * s);

//creates query 1 (csv and html files).
void q1(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 2 (csv and html files).
void q2(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 3 (csv and html files).
void q3(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 4 (csv and html files).
void q4(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//creates query 5 (csv and html files).
void q5(queryADT q, FILE * csvQuery, htmlTable tableQuery );

//frees allocated memory.
void freeQuery(queryADT q);
#endif