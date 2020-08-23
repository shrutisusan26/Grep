CC=gcc

grep: grep1.o main.o 
	$(CC) -o grep grep1.o main.o
grep1.o: grep1.c
	$(CC) -c grep1.c
main.o: main.c
	$(CC) -c main.c
clean: 
	rm -f *.o grep1 main grep
