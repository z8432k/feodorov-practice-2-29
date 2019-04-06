PROG = trapezoid

CFLAGS = -Wall -Wextra -ggdb -ansi -O0
LDLIBS = -lgfortran -lncurses

default: bin/$(PROG)

bin/$(PROG): bin/$(PROG).o bin/fcode.o
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

bin/$(PROG).o: src/$(PROG).c
	$(CC) $(CFLAGS) -o $@ -c $<

bin/fcode.o: src/fcode.f
	gfortran -O0 -ggdb -o $@ -c $<

clean:
	rm -fv bin/*.o bin/$(PROG)