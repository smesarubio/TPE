#include <stdio.h>
#include <stdlib.h>
#include "htmlTable.h"
#include "queryTAD.h"
#define MAX_CHARS 15
#define LINES 150

typedef struct oldestM{
    Tdate date;
    size_t time;
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

typedef struct defective{
    size_t ID;
    struct defective * tail;
}Tdefective;

typedef struct oldestSortedList{
    size_t ID;
    struct oldestSortedList * tail;
}TSOld;

typedef struct queryCDT{
    size_t yearFrom;
    size_t yearTo; 
    TYear * years; //list sorted by year.
    TSensor * sensorsID;  //sensors sorted by ID.
    TNodeS * sensorsP; //sensors sorted by pedestrians.
    oldestM oldest[MAX];
    TSOld * sortedOld;//oldest counts sorted by date.
    Tdefective * defective; //defective sensors.
}queryCDT;

queryADT newQuery(size_t yearFrom, size_t yearTo, int * error){
    queryADT q = calloc(1, sizeof(queryCDT));
    if (q == NULL){
        *error = 1;
    }
    q->yearFrom = yearFrom;
    q->yearTo = yearTo;
    return q;
}

//creates a list of sensors sorted by pedestrians.
static TNodeS * sortSensorL(TNodeS * l, size_t pedestrians, TSensor * vecSen, size_t i){
    if (vecSen[i].flag != 'A'){
        return l;
    }
    if(vecSen[i].name != NULL){
        if(l == NULL || l->pedestrians < pedestrians 
            || (l->pedestrians == pedestrians && strcasecmp(vecSen[(l->ID)-1].name, vecSen[i].name) > 0)){
            TNodeS * aux = malloc(sizeof(TNodeS));
            aux->ID = i+1;
            aux->pedestrians = pedestrians;
            aux->tail = l;
            return aux;
        }
        l->tail = sortSensorL(l->tail, pedestrians, vecSen, i);    
    }
    return l;
}

//creates a list of defective sensors sorted by name.
static Tdefective * sortDefectiveL(Tdefective * def, size_t i, TSensor * vec){
    if((def != NULL && vec[(def->ID)-1].name == NULL) || vec[i].name == NULL){
        return def;
    }
    if(def==NULL || strcasecmp(vec[(def->ID)-1].name, vec[i].name)>0){
        Tdefective * new = malloc(sizeof(Tdefective));
        new->ID = i+1;
        new->tail = def;
        return new;
    }
    def->tail = sortDefectiveL(def->tail, i, vec);
    return def;
}

//returns -1 if date 2 is after date 1, 1 if date 1 is after date 2 or 0 if they are the same.
static char dateCmp(Tdate date1, Tdate date2, char * usedFlag){
    if(*usedFlag==0){
        *usedFlag = 1;
        return -1;
    }
    if (date1.year < date2.year)
        return 1;
        else if (date1.year > date2.year)
        return -1;

        if (date1.year == date2.year)
        {
            if (date1.month < date2.month)
                return 1;
            else if (date1.month > date2.month)
                return -1;
            else if (date1.day < date2.day)
                return 1;
            else if(date1.day > date2.day)
                return -1;
            else if(date1.time < date2.time)
                return 1;
            else if(date1.time > date2.time)
                return -1;
            else 
                return 0;
        }
    return 0;
}

//creates a list of id's sorted by their oldest count.
static TSOld * sortOldByDate(oldestM * old,TSOld * list, int index, TSensor * vec){
    char c;
    if(vec[index].flag == 'R' || old[index].used == 0){
        return list;
    }
    if(list == NULL || (c = dateCmp(old[list->ID-1].date, old[index].date, &(old->used))) == -1){
        TSOld * new = malloc(sizeof(TSOld));
        new->ID = index+1;
        new->tail = list;
        return new;
    }
    if(c==0 && strcasecmp(vec[(list->ID)-1].name, vec[index].name)>0){
        TSOld * new = malloc(sizeof(TSOld));
        new->ID = index+1;
        new->tail = list;
        return new;
    }
    list->tail = sortOldByDate(old, list->tail,index, vec);
    return list;
}

//adds years in order to the year list.
static TYear * addRec(TYear * years, size_t total, size_t year, size_t day, int * error){
    if ( years == NULL || years->year < year){ 
        TYear * new = malloc(sizeof(TYear));
        if (new == NULL){
            *error = 1; 
        }
        new->year = year;
        new->total = total;
        if (day){
            new->Dweekend = total;
            new->Dweek = 0;
        }else {
            new->Dweek = total;
            new->Dweekend = 0;
        }
        new->tail = years;
        return new;
    }
    if(years->year == year){
        years->total += total;
        if (day){
            years->Dweekend += total;
        }else {
            years->Dweek += total;
        }
        return years;
    }
    years->tail = addRec(years->tail, total, year, day, error); 
    return years;
}

void addYear(queryADT query, size_t count, Tdate date, size_t time, size_t day, size_t ID, int * error){
    if (query->sensorsID[ID - 1].name != NULL && query->sensorsID[ID - 1].flag == 'A'){
        query->sensorsID[ID - 1].total += count;
        addOldest(query, ID, date, count, time);
        query->years = addRec(query->years, count, date.year, day, error);
    }
}

void printVec(queryADT q){
    for (int i =0; i<MAX; i++){
        printf("%s\n",q->sensorsID[i].name);
    }
}


void addSensor(size_t ID, char * name, char * value, queryADT q, int * flag, int * error){
    if(*flag == 0){
        q->sensorsID = calloc(MAX, sizeof(TSensor));
        *flag = 1;
    }
    q->sensorsID[ID - 1].len = strlen(name);
    q->sensorsID[ID - 1].name = malloc(q->sensorsID[ID - 1].len + 1);
    if (q->sensorsID[ID - 1].name == NULL) {
        *error = 1;
    }
    strcpy(q->sensorsID[ID - 1].name, name);
    q->sensorsID[ID - 1].flag = *value;
    q->sensorsID[ID - 1].total = 0;
    q->sensorsID[ID - 1].defective = 0;
}

void addOldest(queryADT q, size_t ID, Tdate date, size_t pedestrians, size_t time){
    int c = dateCmp(q->oldest[ID-1].date, date, &(q->oldest[ID-1].used));
    if(c==-1){
        q->oldest[ID-1].date.year = date.year;
        q->oldest[ID-1].old_count = pedestrians;
        q->oldest[ID-1].date.day = date.day;
        q->oldest[ID-1].date.month = date.month;
        q->oldest[ID-1].time = time;
    }
    if(date.year >= q->yearFrom && date.year <= q->yearTo){
        q->sensorsID[ID-1].defective = 1; 
    }
}

void makeSenL(queryADT q){
    for(int i = 0; i < MAX; i++){
        q->sensorsP = sortSensorL(q->sensorsP, q->sensorsID[i].total, q->sensorsID, i);
        if(q->sensorsID[i].defective == 0){
            q->defective = sortDefectiveL(q->defective, i, q->sensorsID );
        }
        if (q->sensorsID[i].name != NULL){
            q->sortedOld = sortOldByDate(q->oldest,q->sortedOld, i, q->sensorsID);
        }
    }
}

size_t dayToNum(char * s){            
    return (s[0] == 'S' || s[0] == 's') ? 1:0; 
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

void q1(queryADT q, FILE * csvQuery, htmlTable tableQuery ){
    TNodeS * aux = q->sensorsP;
    while(aux != NULL){
        fprintf(csvQuery, "%s; %li\n", q->sensorsID[aux->ID-1].name,q->sensorsID[aux->ID-1].total);
        char c[MAX_CHARS];
        sprintf(c, "%li", q->sensorsID[aux->ID-1].total);
        addHTMLRow(tableQuery, q->sensorsID[aux->ID-1].name, c );
        aux = aux->tail;
    }
    return;
}

void q2(queryADT q, FILE * csvQuery, htmlTable tableQuery ){
    TYear * aux = q->years;
    while(aux != NULL){
        fprintf(csvQuery, "%li; %li; %li; %li\n",aux->year,aux->Dweek,aux->Dweekend,aux->total);
        char a[MAX_CHARS],b[MAX_CHARS],c[MAX_CHARS],d[MAX_CHARS];
        sprintf(a, "%li",aux->year);
        sprintf(b, "%li",aux->Dweek);
        sprintf(c, "%li",aux->Dweekend);
        sprintf(d, "%li",aux->total);
        addHTMLRow(tableQuery, a,b,c,d);
        aux = aux->tail;
    }
    return;
}

void q3(queryADT q, FILE * csvQuery, htmlTable tableQuery ){
    TYear *aux = q->years;
    while (aux != NULL) {
        if ((aux->year) % 4 == 0) {
        float i = (float)aux->total / 366.0;
        fprintf(csvQuery, "%li; %.2f\n", aux->year, i);
        char y[MAX], a[MAX];
        sprintf(y, "%li", aux->year);
        sprintf(a, "%.2f", i);
        addHTMLRow(tableQuery, y, a);
        } else {
        float j = (float)aux->total / 365.0;
        fprintf(csvQuery, "%li; %.2f\n", aux->year, j);
        char y2[MAX], a2[MAX];
        sprintf(y2, "%li", aux->year);
        sprintf(a2, "%.2f", j);
        addHTMLRow(tableQuery, y2, a2);
        }
        aux = aux->tail;
    }
}

void q4(queryADT q, FILE * csvQuery, htmlTable tableQuery ){
    Tdefective * aux = q->defective;
    while(aux != NULL){
        fprintf(csvQuery, "%s\n", q->sensorsID[aux->ID-1].name);
        addHTMLRow(tableQuery, q->sensorsID[aux->ID-1].name);
        aux = aux->tail;
    }
    return;
}

void q5(queryADT q, FILE * csvQuery, htmlTable tableQuery ){
    TSOld * aux = q->sortedOld;
    while(aux != NULL){
        char a[MAX_CHARS],b[MAX_CHARS],c[MAX_CHARS];
        sprintf(a, "%li/%li/%li",q->oldest[aux->ID-1].date.day, q->oldest[aux->ID-1].date.month, q->oldest[aux->ID-1].date.year);
        sprintf(b, "%li",q->oldest[aux->ID-1].time);
        sprintf(c, "%li",q->oldest[aux->ID-1].old_count);
        fprintf(csvQuery, "%s; %li; %s; %li\n",a,q->oldest[aux->ID-1].time,q->sensorsID[aux->ID-1].name, q->oldest[aux->ID-1].old_count);
        addHTMLRow(tableQuery, a,b,q->sensorsID[aux->ID-1].name,c);
        aux = aux->tail;
    }
    return;
}

//frees years list.
static void freeRecYears(TYear * years){
    if(years == NULL){
        return;
    }
    freeRecYears(years->tail);
    free(years);
}

//frees sensors list.
static void freeRecSen(TNodeS * l){
    if(l==NULL){
        return;
    }
    freeRecSen(l->tail);
    free(l);
}

//free defective list.
static void freeRecDef(Tdefective * l){
    if(l==NULL){
        return;
    }
    freeRecDef(l->tail);
    free(l);
}

//free oldest list.
static void freeRecOld(TSOld * l){
    if(l==NULL){
        return;
    }
    freeRecOld(l->tail);
    free(l);
}

void freeQuery(queryADT q) {
    for (int j=0; j<MAX; j++){
        free(q->sensorsID[j].name);
    }
    free(q->sensorsID);
    freeRecOld(q->sortedOld);
    freeRecYears(q->years);
    freeRecSen(q->sensorsP); 
    freeRecDef(q->defective);
    free(q);    
}

