CC=gcc
CFLAGS=-g
LIBS = -lpthread -L ./CommandParser -lcli 
OBJS=gluethread/glthread.o graph.o topologies.o net.o utils.o nwcli.o

test: testapp.o ${OBJS} CommandParser/libcli.a 
	${CC} ${CFLAGS} testapp.o ${OBJS} -o test ${LIBS}

testapp.o: testapp.c 
	$(CC) $(CFLAGS) -c testapp.c -o testapp.o

gluethread/glthread.o: gluethread/glthread.c 
	$(CC) $(CFLAGS) -c -I gluethread gluethread/glthread.c -o gluethread/glthread.o

graph.o: graph.c
	$(CC) $(CFLAGS) -c -I . graph.c -o graph.o

topologies.o: topologies.c 
	$(CC) $(CFLAGS) -c -I . topologies.c -o topologies.o

net.o: net.c 
	$(CC) $(CFLAGS) -c -I . net.c -o net.o

utils.o: utils.c 
	$(CC) $(CFLAGS) -c -I . utils.c -o utils.o

CommandParser/libcli.a: 
	(cd CommandParser; make)
	
clean:
	rm gluethread/glthread.o
	rm *.o
	rm test
	(cd CommandParser; make clean)
all: 
	make
	(cd CommandParser; make)
	
    
