# TPE
Trabajo Final de PI.
Para crear los ejecutables: Tener en el mismo directorio los archivos main.c, queryTAD.c, 
queryTAD.h, Makefile, htmlTable.c y htmlTable.h. Luego, ejecutar el comando make en terminal
(es necesario tener gcc instalado).
Para ejecutarlos se debe correr en terminal el comando:
    ./pedestrians sensors.csv readings.csv <year1> <year2> 
donde "year1" y "year2" son los años entre los que se quiere ver los sensores defectuosos. en 
caso de solo poner "year1" se verán desde ese año hasta el presente y en caso de no poner ningún año
se verán todos los años.
