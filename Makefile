PROGRAM=./rust_in_pieces

all: $(PROGRAM)

$(PROGRAM): scanner.l parser.y common.h ast.h symlist.h
	bison -d -v parser.y
	flex -o scanner.c scanner.l
	gcc -o $(PROGRAM) parser.tab.c scanner.c ast.c symlist.c -lfl

clean:
	rm -f $(PROGRAM) parser.tab.* parser.output scanner.c
