.PHONY:clean

CC=gcc
OBJ=client.o interface.o config.o

client:$(OBJ)
	$(CC) $^ -o $@
	
%.o:%.c
	$(CC) $< -c
	
clean:
	rm -f $(OBJ) client
