#include "htmlTable.h"
#include "queryTAD.h"
#include <time.h>

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
    createSensorV(fSensor, query);
    createYearL(fReadings, query);
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


