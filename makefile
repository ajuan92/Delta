LIBS = -lm
CC = gcc
CFLAGS = pwm.out  pwm.h  
OUTPUT = DeltaK.out
SOURCES = Deltak.c

all:

	$(CC) $(SOURCES) $(LIBS) $(CFLAGS) -o $(OUTPUT)
clean:

	rm -f $(OUTPUT)
fresh:

	$(clean) $(all)
