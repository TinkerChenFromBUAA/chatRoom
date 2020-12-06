.PHONY:clean


CC=gcc
OBJ=server.o config.o
LIB=-pthread

server:$(OBJ)
	$(CC) $^ -o $@ $(LIB)

%.o:%.c
	$(CC) $< -c
	
clean:
	rm -f *.o server
