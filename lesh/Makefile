CC      = clang
CFLAGS  = -g -Wall
RM      = rm -f


default: run

build: src/main.c src/history/history.c src/parser/parser.c
	$(CC) $(CFLAGS) -o lesh src/main.c src/history/history.c src/parser/parser.c

run: build
	./lesh

clean:
	$(RM) lesh
