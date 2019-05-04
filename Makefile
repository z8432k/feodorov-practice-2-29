PROG = trapezoid

CFLAGS = -Wall -Wextra -ggdb -ansi -O0 `pkg-config --cflags glib-2.0`
LDLIBS = -lgfortran -lncurses `pkg-config --libs glib-2.0`

default: bin/$(PROG)

bin/$(PROG): bin/$(PROG).o bin/integral_trapezoid.o
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

bin/$(PROG).o: src/$(PROG).c
	$(CC) $(CFLAGS) -o $@ -c $<

bin/integral_trapezoid.o: src/integral_trapezoid.f
	gcc -O0 -ggdb -o $@ -c $<

clean:
	rm -fv bin/*.o bin/$(PROG)
