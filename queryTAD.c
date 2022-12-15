#include <stdio.h>
#include <stdlib.h>
#include "queryTAD.h"

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
    TYear * years; //lista por año
    TSensor * sensorsID;  //sensores ordenados por ID
    TNodeS * sensorsP; //sensores ordenados por peatones
    oldestM oldest[MAX];
    TSOld * sortedOld;//lista de mediciones viejas ordenadas por fecha
    Tdefective * defective; //lista de sensores defectuosos
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
}

//returns 1 for weekend or 0 for weekday
size_t dayToNum(char * s){    
    if (s == NULL){
        return 0;
    }           
    return s[0] == 'S' || s[0] == 's'; 
}

size_t monthToNum (char * s){
    if (s == NULL){
        return 0;
    }
    char *months[] = {"January", "February", "March", "April","May", "June", "July", "August", "September", "October", "November", "December"};
    for (int i=0; i<12; i++){
        if (strcasecmp(s, months[i]) == 0){
            return i+1;
        }
    }
    return -1;
}

/*static char dateCmp(size_t year1, size_t year2, size_t month1, size_t day1, size_t month2, size_t day2, char * usedFlag,size_t time1 , size_t time2){
    if (year1 == NULL || year2 == NULL || month1 == NULL || month2 == NULL || day1 == NULL || day2 == NULL){
        return -1;
    }
    if(*usedFlag==0){
        *usedFlag = 1;
        return -1;
    }
    if(year1==year2 && month1 == month2 && day1==day2 &&  time1 == time2){
        return 0; // mismo día
    } 
    if(year1 == year2 && month1 == month2)
    if (year1 < year2){
        return 1;
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
}*/
static char dateCmp(size_t year1, size_t year2, size_t month1, size_t day1, size_t month2, size_t day2, char * usedFlag,size_t time1 , size_t time2){
    if(*usedFlag==0){
        *usedFlag = 1;
        return -1;
    }
    
    if (year1 < year2)
        return 1;
        else if (year1 > year2)
        return -1;

        if (year1 == year2)
        {
            if (month1<month2)
                return 1;
            else if (month1 > month2)
                return -1;
            else if (day1<day2)
                return 1;
            else if(day1>day2)
                return -1;
            else if(time1 < time2)
                return 1;
            else if(time1 > time2)
                return -1;
            else 
                return 0;
        }
    return 0;
}

static TSOld * sortOldByDate(oldestM * old,TSOld * list, int index, TSensor * vec){
    char c;
    if(vec[index].flag == 'R' || old[index].used == 0){
        return list;
    }
    if(list == NULL || (c = dateCmp(old[list->ID-1].year, old[index].year, old[list->ID-1].month ,old[list->ID-1].dayN , old[index].month,old[index].dayN,&(old->used),old[list->ID-1].time, old[index].time))==-1){
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
void addOldest(queryADT q, size_t ID, size_t month, size_t dayN, size_t time, size_t pedestrians, size_t year){
    if (q->sensorsID[ID - 1].name == NULL){
        return;
    }
    int c = dateCmp(q->oldest[ID-1].year, year,q->oldest[ID-1].month, month, q->oldest[ID-1].dayN, dayN, &(q->oldest[ID-1].used),q->oldest[ID-1].time, time);
    // printf("%i\n", c);
    if(c==-1){
        q->oldest[ID-1].year = year;
        q->oldest[ID-1].old_count = pedestrians;
        q->oldest[ID-1].dayN = dayN;
        q->oldest[ID-1].month = month;
        q->oldest[ID-1].time = time;
        /*printf("%10lu/", q->oldest[ID-1].dayN);
        printf("%lu/", q->oldest[ID-1].month);
        printf("%lu|\t", q->oldest[ID-1].year);
        printf("time: %10lu|\t", q->oldest[ID-1].time);
        printf("peds: %10lu|\n", q->oldest[ID-1].old_count);
    */
    }
    if(year>q->yearFrom && year < q->yearTo){
        q->sensorsID[ID-1].defective = 1; 
    }
}

void insertYearL(queryADT query, TYear * years){
    query->years = years; 
}

static TNodeS * sortSensorL(TNodeS * l, size_t pedestrians, TSensor * vecSen, size_t i){
    // printf("%li\t", i);
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

void makeSenL(queryADT q){
    for(int i = 0; i < MAX; i++){
        q->sensorsP = sortSensorL(q->sensorsP, q->sensorsID[i].total, q->sensorsID, i);
        if(q->sensorsID[i].defective == 0){
            q->defective = sortDefectiveL(q->defective, i, q->sensorsID );
        }
    }

    for(int i=0; i<MAX;i++){
        if (q->sensorsID[i].name != NULL){
            q->sortedOld = sortOldByDate(q->oldest,q->sortedOld, i, q->sensorsID);
        }
    }

    //TNodeS * aux = q->sensorsP;
    /*int j=0;
    while(q->sensorsP != NULL){
        printf("NAME:%s\t", q->sensorsID[j].name);
        printf("TOTAL: %lu\n", q->sensorsP->pedestrians);
        j++;
        q->sensorsP = q->sensorsP->tail;
    }*/
    /*********************************/
    /*IMPRIME LA LISTA DE DEFECTUOSOS*/
    /*********************************/
    TSOld * aux = q->sortedOld;
    while(aux != NULL){
        printf("%s\t", q->sensorsID[ aux->ID-1].name);
        printf("%ld/", q->oldest[ aux->ID-1].dayN);
        printf("%ld/", q->oldest[ aux->ID-1].month);
        printf("%ld\t", q->oldest[aux->ID-1].year);
        printf("TIME: %ld\t", q->oldest[aux->ID-1].time);
        printf("COUNT: %ld\n", q->oldest[aux->ID-1].old_count);
        aux = aux->tail;
    }
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

static void freeRecDef(Tdefective * l){
    if(l==NULL){
        return;
    }
    freeRecDef(l->tail);
    free(l);
}

static void freeRecOld(TSOld * l){
    if(l==NULL){
        return;
    }
    freeRecOld(l->tail);
    free(l);
}

void freeQuery(queryADT q) {
    for (int j=0; j<86; j++){
        free(q->sensorsID[j].name);
    }
    freeRecOld(q->sortedOld);
    freeRecYears(q->years);
    freeRecSen(q->sensorsP); 
    freeRecDef(q->defective);
    free(q->sensorsID);
    free(q);    
}

