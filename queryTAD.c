#include <stdio.h>
#include <stdlib.h>
#include "queryTAD.h"

typedef struct sensor {
    size_t total;
    char * name;
    size_t len;
    char flag;
    char defective;
}TSensor;

typedef struct year {
    size_t total; 
    size_t Dweek;
    size_t Dweekend;
}TYear;

typedef struct queryCDT{
    size_t yearFrom;
    size_t yearTo; 
    TYear * years; //lista por año
    TSensor * sensorsID;  //sensores ordenados por ID
    TSensor * sensorsP; //sensores ordenados por peatones
}queryCDT;



