COMPILE.c       = $(CC) -g -o $@
.SUFFIXES : .c

.c : 
	$(COMPILE.c) $<

PROGS= read insert search delete

all : $(PROGS)

clean :
	rm -f *.o core $(PROGS)
