
#define MAX 86
#include "queryTAD.h"
#define LINES 150

enum month{January = 1, February, March, April, May, June, July, August, September, October, November, December};

char dayToNum(char * s);
TSensor * createSensorL(FILE * fSensor);

int main(int argc, char *argv[]){

    FILE * fSensor = fopen(argv[1], "rt");        
    FILE * fReadings = fopen(argv[2], "rt");
    if (fSensor == NULL || fReadings == NULL){
        perror("Unable to open file.");
        exit(1);
    }       
    queryADT query = newQuery(); 
    TSensor * vectorS = createSensorL(fSensor);
    insertVector(query, vectorS);
    //TYear * years = createYearL(fReadings);
    freeQuery(query);
    fclose(fReadings);
    fclose(fSensor);
    return 0;
}

TSensor * createSensorL(FILE * fSensor){
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
                value = strtok(NULL, ";");
                ans[pos - 1].flag = *value; 
                value = strtok(NULL, ";");
            }
        }
    }
    return ans;
}

//returns 1 for weekend or 0 for weekday
char dayToNum(char * s){               
    return s[0] == 'S' || s[0] == 's'; 
}


size_t monthToNum (char * s){

}

char dateCmp(size_t month1, size_t day1, size_t month2, size_t day2){
    if (month1 < month2){
        return 1;
    }
    if (month1 == month2){
        return day1 < day2;
    }
    return 0;
}

TYear * addRec(TYear * years, TYear * ans){
    if (years == NULL || years->year < ans->year){
        ans->tail = years;
        return ans;
    }
    if(years->year == ans->year){
        years->total += ans->total;
        years->Dweek += ans->Dweek;
        years->Dweekend += ans->Dweekend;
        if(dateCmp(ans->month, ans->dayN, years->month, years->dayN) != 0){  //el nuevo nodo tiene una menor fecha
            years->dayN = ans->dayN;
            years->month = ans->month;
            years->time = ans->time;
            years->old_count = ans->old_count;
        }       
    }
    years->tail = ans;
    return years;
}

TYear * createYearL (FILE * fReadings){
    TYear * list = malloc(sizeof(TYear));
    char line2[LINES];
    fgets(line2, LINES, fReadings);
    while (!feof(fReadings)){
        for (int i =0; fgets(line2, LINES, fReadings); i++){
            char * value = strtok(line2, ";");
            while (value != NULL){
                TYear * years = malloc(sizeof(TYear));
                value = strtok(NULL, ";");
                years->year = atoi(value);
                value = strtok(NULL, ";");
                years->month = monthToNum(value);
                value = strtok(NULL, ";");
                years->day = dayToNum(value);
                value = strtok(NULL, ";");
                years->dayN = atoi(value);
                value = strtok(NULL, ";"); //ID
                value = strtok(NULL, ";"); //TIME 
                value = strtok(NULL, ";"); //COUNTS 
                addRec(list, years);
                value = strtok(NULL, ";"); //siguiente
                free(years); 
            }
        }
    }
}

