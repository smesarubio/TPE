#include "htmlTable.h"
#include "queryTAD.h"
#include <time.h>
#define LINES 150
#define ESNULL(x) ((x)==NULL ? 0:1)

//enum month{January = 1, February, March, April, May, June, July, August, September, October, November, December};

size_t dayToNum(char * s);
TYear * createYearL (FILE * fReadings, queryADT query, TSensor * vec);
TSensor * createSensorV(FILE * fSensor);
void query1(queryADT q);
void query2(queryADT q);
void query3(queryADT q);
void query4(queryADT q);
void query5(queryADT q);


int main(int argc, char *argv[]){
    if (strcmp(argv[2], "readings.csv") != 0 || strcmp(argv[1], "sensors.csv") != 0){
        perror("Incorrect parameters.");
        exit(1);
    }
    FILE * fSensor = fopen(argv[1], "rt");        
    FILE * fReadings = fopen(argv[2], "rt");
    if (fSensor == NULL || fReadings == NULL){
        perror("Unable to open file.");
        exit(1);
    } 

    size_t yearFrom, yearTo;   
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (argc == 3){
        yearFrom = 1900;
        yearTo = tm.tm_year;
    } else if (argc == 4){
        yearFrom = atoi(argv[3]);
        yearTo = tm.tm_year;
    } else {
        yearFrom = atoi(argv[3]);
        yearTo = atoi(argv[4]);
        if (yearTo < yearFrom){
        perror("Incorrect parameters.");
        exit(1);
        }
    }

    queryADT query = newQuery(yearFrom, yearTo); 
    TSensor * vectorS = createSensorV(fSensor);
    insertVector(query, vectorS);
    TYear * years = createYearL(fReadings, query, vectorS);
    insertYearL(query, years);
    makeSenL(query);

    query1(query);
    query2(query);
    query3(query);
    query4(query);
    query5(query);

    freeQuery(query);
    fclose(fReadings);
    fclose(fSensor);
    return 0;
}

void query1(queryADT q){
    FILE * query = fopen("query1.csv", "wt");
    htmlTable table = newTable("query1.html", 2, "Sensor", "Pedestrians");
    fprintf(query, "Sensor; Pedestrians\n");
    q1(q, query, table);
    fclose(query);
    closeHTMLTable(table);
    return;
}

void query2(queryADT q){
    FILE * query = fopen("query2.csv", "wt");
    htmlTable table = newTable("query2.html", 4, "Year", "Weekdays", "Weekend", "Total");
    fprintf(query, "Year; Weekdays; Weekend; Total\n");
    q2(q, query, table);
    fclose(query);
    closeHTMLTable(table);
    return;
}

void query3(queryADT q){
    FILE * query = fopen("query3.csv", "wt");
    htmlTable table = newTable("query3.html", 2, "Year", "Pedestrian Avg");
    fprintf(query, "Year; Pedestrian Avg\n");
    q3(q, query, table);
    fclose(query);
    closeHTMLTable(table);
    return;
}

void query4(queryADT q){
    FILE * query = fopen("query4.csv", "wt");
    htmlTable table = newTable("query4.html", 1, "Sensor");
    fprintf(query, "Sensor\n");
    q4(q, query, table);
    fclose(query);
    closeHTMLTable(table);
    return;
}

void query5(queryADT q){
    FILE * query = fopen("query5.csv", "wt");
    htmlTable table = newTable("query5.html", 4, "Oldest Date", "Oldest Hour", "Sensor", "Oldest Count" );
    fprintf(query, "Oldest Date; Oldest Hour; Sensor; Oldest Count\n");
    q5(q, query, table);
    fclose(query);
    closeHTMLTable(table);
    return;
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
        for (int i=0; fgets(line, LINES, fSensor); i++){ //id, name, status
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

TYear * addRec(TYear * years, TYear * ans){
    if(ans==NULL){
        return years;
    }
    if ( years == NULL || years->year < ans->year){ 
        ans->tail = years;
        return ans;
    }
    if(years->year == ans->year){
        years->total += ans->total;
        years->Dweek += ans->Dweek;
        years->Dweekend += ans->Dweekend;
        free(ans);
        return years;
    }
    years->tail = addRec(years->tail, ans); 
    return years;
}

TYear * createYearL (FILE * fReadings, queryADT query, TSensor * vec){
    TYear * list = NULL;
    char line2[LINES];
    fgets(line2, LINES, fReadings);
    while (!feof(fReadings)){
        for (int i = 0; fgets(line2, LINES, fReadings); i++){
            char * value = strtok(line2, ";");//YEAR
            while (value != NULL){
                size_t month, dayN, ID, count, time, day, year;
                TYear * years = calloc(1, sizeof(TYear));
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
                if (vec[ID - 1].name != NULL && vec[ID - 1].flag == 'A'){
                    years->total += count;
                    vec[ID - 1].total += count;
                    if(day){
                        years->Dweekend += count;
                    } else{
                        years->Dweek += count;
                    }
                    Tdate date = {dayN, month, year, time};
                    addOldest(query, ID, date, count);
                }
                list = addRec(list, years);
                value = strtok(NULL, ";");
            }
        }
    }
    return list;
}

