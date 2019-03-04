PROG = trapezoid

CFLAGS = -Wall -g -m32 -ansi -O0
LDLIBS =

default: bin/$(PROG)

bin/$(PROG): bin/$(PROG).o
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

bin/$(PROG).o: src/$(PROG).c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -fv bin/*.o bin/$(PROG)
