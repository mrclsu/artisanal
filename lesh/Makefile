CC      = clang
CFLAGS  = -g -Wall
RM      = rm -f


default: run

build: src/main.c
	$(CC) $(CFLAGS) -o lesh src/main.c

run: build
	./lesh

clean:
	$(RM) lesh
