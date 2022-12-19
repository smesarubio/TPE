CC = gcc
ALL_CFLAGS = -pedantic -Wall -std=c99 

pedestrians: main.o queryTAD.o ctable.o
	$(CC) $(ALL_CFLAGS) -o $@ $^
	make clean

main.o: main.c
	$(CC) $(ALL_CFLAGS) -c -o $@ $^

queryTAD.o: queryTAD.c
	$(CC) $(ALL_CFLAGS) -c -o $@ $^

ctable.o: htmlTable.c
	$(CC) $(ALL_CFLAGS) -c -o $@ $^

clean:
	rm *.o
# Example:
# dist/newfilename.o: src/filename.c
# 	$(CC) $(ALL_CFLAGS) -c -o $@ $^