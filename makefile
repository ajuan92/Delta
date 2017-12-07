LIBS = -lm -pthread
CC = gcc
CFLAGS = -Wall -Werror  
OUTPUT = Delta.out
SOURCES = Server.c DeltaCon.c DeltaK.c pwm.c

all:

	$(CC) $(SOURCES) $(LIBS) $(CFLAGS) -o $(OUTPUT)
clean:

	rm -f $(OUTPUT)
fresh:

	$(clean) $(all)
