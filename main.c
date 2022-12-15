#include "htmlTable.h"
#include "queryTAD.h"
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
    TYear * years = createYearL(fReadings, query, vectorS);
    insertYearL(query, years);
    makeSenL(query);
    /*for (int i=0; i<MAX; i++){
        printf("ID: %d\t", i+1);
        printf("TOTAL: %li\t", vectorS[i].total);
        printf("FLAG: %c\t", vectorS[i].flag);
        printf("NAME: %s\n", vectorS[i].name);
        printf("OLDEST M: %li\t", ans[i].oldest.month);
        printf("OLDEST D: %li\t", ans[i].oldest.dayN);
        printf("OLDEST T: %li\t", ans[i].oldest.time);
        printf("OLDEST C: %li\n", ans[i].oldest.old_count);
    }
    while (years != NULL){
        printf("YEAR: %li\t", years->year);
        printf("WEEK: %li\t", years->Dweek);
        printf("WEEKEND: %li\t", years->Dweekend);
        printf("TOTAL: %li\n", years->total);
        years = years->tail;
    }
    */
    query1(query);
    query2(query);
    query3(query);
    query4(query);
    query5(query);

    // query5(query);

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
        for (int i=0; fgets(line, LINES, fSensor); i++){ //id, name,status
            char * value = strtok(line, ";");
            while (value != NULL){
                size_t pos = atoi(value);
                value = strtok(NULL, ";");
                if (value != NULL){
                    ans[pos - 1].len = strlen(value);
                    ans[pos - 1].name = malloc(ans[pos - 1].len + 1);
                    if (ans[pos - 1].name == NULL) {
                        perror("Unable to allocate memory.");
                        exit(1);
                    }
                    strcpy(ans[pos - 1].name, value);
                }
                value = strtok(NULL, ";");
                if (value != NULL){
                    ans[pos - 1].flag = *value; 
                }
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

TYear * createYearL (FILE * fReadings, queryADT query, TSensor * vec){
    TYear * list = NULL;
    char line2[LINES];
    fgets(line2, LINES, fReadings);
    while (!feof(fReadings)){
        for (int i = 0; fgets(line2, LINES, fReadings); i++){
            char * value = strtok(line2, ";");//YEAR
            while (value != NULL){
                size_t month, dayN, ID, count, time, day, year, flag = 1;
                TYear * years = calloc(1, sizeof(TYear));
                if (value != NULL){
                    year = atoi(value);
                    years->year = year;
                }else {
                    flag = 0;
                }
                value = strtok(NULL, ";");//MONTH
                month = monthToNum(value);
                value = strtok(NULL, ";"); //DAYN
                if (value != NULL){
                    dayN = atoi(value);
                }else {
                    flag = 0;
                }
                value = strtok(NULL, ";");//DAY
                day = dayToNum(value);
                value = strtok(NULL, ";"); //ID
                if (value != NULL){
                    ID = atoi(value);
                }else {
                    flag = 0;
                }
                value = strtok(NULL, ";"); //TIME 
                if (value != NULL){
                    time = atoi(value);
                }else {
                    flag = 0;
                }
                value = strtok(NULL, ";"); //COUNTS 
                if (value != NULL){
                    count = atoi(value);
                    if (vec[ID - 1].name != NULL && vec[ID - 1].flag == 'A'){
                        years->total += count;
                        vec[ID - 1].total += count;
                        if(day){
                            years->Dweekend += count;
                        } else{
                            years->Dweek += count;
                        }
                        if (flag){
                            addOldest(query, ID, month, dayN, time, count, year);
                        }
                    }
                }
                list = addRec(list, years);
                value = strtok(NULL, ";"); //siguiente
                //free(years); 
            }
        }
    }
    return list;
}

