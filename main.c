#include "htmlTable.h"
#include "queryTAD.h"
#include <time.h>
#define LINES 150
#define delimitador ';'

//creates an array of sensors sorted by ID.
void createSensorV(FILE * fSensor, queryADT q, int * error);
//creates a list sorted by year.
void createYearL (FILE * fReadings, queryADT query, int * error);
void query1(queryADT q);
void query2(queryADT q);
void query3(queryADT q);
void query4(queryADT q);
void query5(queryADT q);
void errorCheck(int a);

int main(int argc, char *argv[]){
    int error = 0;
    if (strcmp(argv[2], "readings.csv") != 0 || strcmp(argv[1], "sensors.csv") != 0){
        error = 2;
        errorCheck(error);
    }
    FILE * fSensor = fopen(argv[1], "rt");        
    FILE * fReadings = fopen(argv[2], "rt");
    if (fSensor == NULL || fReadings == NULL){
        error = 3;
        errorCheck(error);
    } 
    size_t yearFrom, yearTo;   
    time_t t = time(NULL);
    struct tm * tm = localtime(&t);
    if (argc == 3){
        yearFrom = 1900; 
        yearTo = tm->tm_year + 1900;
    } else if (argc == 4){
        yearFrom = atoi(argv[3]);
        if (yearFrom == 0){ //example: atoi("dosmil") returns 0
            error = 2;
        }
        yearTo = tm->tm_year + 1900;
    } else {
        yearFrom = atoi(argv[3]);
        yearTo = atoi(argv[4]);
        if (yearTo < yearFrom || yearFrom == 0 || yearTo == 0){ //example: atoi("dosmil") returns 0
        error = 2;
        }
    }
    errorCheck(error);
    queryADT query = newQuery(yearFrom, yearTo, &error); 
    errorCheck(error);
    createSensorV(fSensor, query, &error);
    createYearL(fReadings, query, &error);
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

void createSensorV(FILE * fSensor, queryADT q, int * error){
    size_t ID;
    char * name;
    int flag = 0;
    char line[LINES];
    fgets(line, LINES, fSensor);
    while (!feof(fSensor)){
        for (int i=0; fgets(line, LINES, fSensor); i++){ //id, name, status
            char * value = strtok(line, ";");
            while (value != NULL){
                ID = atoi(value);
                value = strtok(NULL, ";");
                name = value;
                value = strtok(NULL, ";");
                addSensor(ID, name, value, q, &flag, error);
                value = strtok(NULL, ";");
            }
        }
    }
    errorCheck(error);
}

void createYearL (FILE * fReadings, queryADT query, int * error){
    char line2[LINES];
    fgets(line2, LINES, fReadings);
    while (!feof(fReadings)){
        for (int i = 0; fgets(line2, LINES, fReadings); i++){
            char * value = strtok(line2, ";");//YEAR
            while (value != NULL){
                size_t month, dayN, ID, count, time, day, year;
                year = atoi(value);
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
                Tdate date = {dayN, month, year, time};
                addYear(query, count, date, time, day, ID, error);
                value = strtok(NULL, ";");
            }
        }
    }
    errorCheck(error);

}

void query1(queryADT q){
    FILE * query1 = fopen("query1.csv", "wt");
    htmlTable table1 = newTable("query1.html", 2, "Sensor", "Pedestrians");
    fprintf(query1, "Sensor; Pedestrians\n");
    q1(q, query1, table1);
    fclose(query1);
    closeHTMLTable(table1);
    return;
}

void query2(queryADT q){
    FILE * query2 = fopen("query2.csv", "wt");
    htmlTable table2 = newTable("query2.html", 4, "Year", "Weekdays", "Weekend", "Total");
    fprintf(query2, "Year; Weekdays; Weekend; Total\n");
    q2(q, query2, table2);
    fclose(query2);
    closeHTMLTable(table2);
    return;
}

void query3(queryADT q){
    FILE * query3 = fopen("query3.csv", "wt");
    htmlTable table3 = newTable("query3.html", 2, "Year", "Pedestrian Avg");
    fprintf(query3, "Year; Pedestrian Avg\n");
    q3(q, query3, table3);
    fclose(query3);
    closeHTMLTable(table3);
    return;
}

void query4(queryADT q){
    FILE * query4 = fopen("query4.csv", "wt");
    htmlTable table4 = newTable("query4.html", 1, "Sensor");
    fprintf(query4, "Sensor\n");
    q4(q, query4, table4);
    fclose(query4);
    closeHTMLTable(table4);
    return;
}

void query5(queryADT q){
    FILE * query5 = fopen("query5.csv", "wt");
    htmlTable table5 = newTable("query5.html", 4, "Oldest Date", "Oldest Hour", "Sensor", "Oldest Count" );
    fprintf(query5, "Oldest Date; Oldest Hour; Sensor; Oldest Count\n");
    q5(q, query5, table5);
    fclose(query5);
    closeHTMLTable(table5);
    return;
}

void errorCheck(int a){
    switch (a)
    {
    case 0:
        return; 
        break;    
    case 1:
        perror("Unable to allocate memory");
        exit(1);
        break;
    case 2:
        perror("Incorrect parameters");
        exit(1);
    case 3:
        perror("Unable to open files");
        exit(1);
    }
}
