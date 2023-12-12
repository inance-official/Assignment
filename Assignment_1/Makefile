COMPILE.c		= $(CC) -g -o $@
.SUFFIXES : .c

.c :
	$(COMPILE.c) $<

PROGS= file

all : $(PROGS)

clean :
	rm -f *.o core $(PROGS)
