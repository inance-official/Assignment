#CC					= gcc
#CFLAGS				= -W -Wall -O2
#LDFLAGS			= -lm

#file : file.c
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

#clean :
#	@rm -f *.o file

COMPILE.c		= $(CC) -g -o $@
.SUFFIXES : .c

.c : 
	$(COMPILE.c) $<

PROGS= file

all : $(PROGS)

clean :
	rm -f *.o core $(PROGS)
