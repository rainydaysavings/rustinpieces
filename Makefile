PROGRAM=./rust_in_pieces

all: $(PROGRAM)

$(PROGRAM): scanner.l parser.y common.h ast.h symlist.h code.h mips.h
	bison -d -v parser.y
	flex -o scanner.c scanner.l
	gcc -o $(PROGRAM) parser.tab.c scanner.c ast.c symlist.c code.c mips.c -lfl

clean:
	rm -f $(PROGRAM) parser.tab.* parser.output scanner.c

