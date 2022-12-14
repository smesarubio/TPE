
#define MAX 86
#include "queryTAD.h"
#define LINES 150

//enum month{January = 1, February, March, April, May, June, July, August, September, October, November, December};

size_t dayToNum(char * s);
TYear * createYearL (FILE * fReadings, queryADT query);
TSensor * createSensorV(FILE * fSensor);

int main(int argc, char *argv[]){
    FILE * fSensor = fopen(argv[1], "rt");        
    FILE * fReadings = fopen(argv[2], "rt");
    if (fSensor == NULL || fReadings == NULL){
        perror("Unable to open file.");
        exit(1);
    }       
    size_t yearFrom = atoi(argv[3]);
    size_t yearTo = atoi(argv[4]);
    queryADT query = newQuery(yearFrom, yearTo); 
    TSensor * vectorS = createSensorV(fSensor);
    insertVector(query, vectorS);
    TYear * years = createYearL(fReadings, query);
    insertYearL(query, years);
    freeQuery(query);
    fclose(fReadings);
    fclose(fSensor);
    return 0;
}

TSensor * createSensorV(FILE * fSensor){
    TSensor * ans = calloc(MAX, sizeof(TSensor));
    if (ans == NULL){
        perror("Unable to allocate memory.");
        exit(1);
    }
    char line[LINES];
    fgets(line, LINES, fSensor);
    while (!feof(fSensor)){
        for (int i=0; fgets(line, LINES, fSensor); i++){ //id, name,status
            char * value = strtok(line, ";");
            while (value != NULL){
                size_t pos = atoi(value);
                value = strtok(NULL, ";");
                ans[pos - 1].len = strlen(value);
                ans[pos - 1].name = malloc(ans[pos - 1].len + 1);
                if (ans[pos - 1].name == NULL) {
                    perror("Unable to allocate memory.");
                    exit(1);
                }
                strcpy(ans[pos - 1].name, value);
                ans[pos-1].oldest.used = 0;
                value = strtok(NULL, ";");
                ans[pos - 1].flag = *value; 
                value = strtok(NULL, ";");
            }
        }
    }
    return ans;
}

TYear * addRec(TYear * years, TYear * ans){
    if(ans==NULL){
        return years;
    }
    if ( years == NULL || years->year < ans->year){  //el nodo no existia, lo agrego
        ans->tail = years;
        return ans;
    }
    if(years->year == ans->year){   //ya existia el nodo
        years->total += ans->total;
        years->Dweek += ans->Dweek;
        years->Dweekend += ans->Dweekend;
        free(ans);
        return years;
    }
    years->tail = addRec(years->tail, ans);  //sigo buscando
    return years;
}

TYear * createYearL (FILE * fReadings, queryADT query){
    TYear * list = NULL;
    char line2[LINES];
    fgets(line2, LINES, fReadings);
    while (!feof(fReadings)){
                printf("HOLA\n");
        for (int i = 0; fgets(line2, LINES, fReadings); i++){
            char * value = strtok(line2, ";");//YEAR
            while (value != NULL){
                size_t month, dayN, ID, count, time, day, year;
                TYear * years = malloc(sizeof(TYear));
                year = atoi(value);
                years->year = year;
                value = strtok(NULL, ";");//MONTH
                month = monthToNum(value);
                value = strtok(NULL, ";"); //DAYN
                dayN = atoi(value);
                value = strtok(NULL, ";");//DAY
                day = dayToNum(value);
                value = strtok(NULL, ";"); //ID
                ID = atoi(value);
                value = strtok(NULL, ";"); //TIME 
                time = atoi(value);
                value = strtok(NULL, ";"); //COUNTS 
                count = atoi(value);
                if(day){
                    years->Dweekend += count;
                } else{
                    years->Dweek += count;
                }
                years->total += count;
                list = addRec(list, years);
                addOldest(query, ID, month, dayN, time, count, year);
                value = strtok(NULL, ";"); //siguiente
                //free(years); 
            }
        }
    }
    return list;
}

