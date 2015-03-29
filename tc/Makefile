YACC=bison -y
LEX=flex
CFLAGS=-g -Wall
LDFLAGS=-g -Wall
LD=$(CC)

PROGS= \
	tc \
	RTS.o

TC_SRC = \
	lex.yy.c \
	y.tab.c \
	ast.c \
	mtrack.c \
	literal.c \
	type.c \
	class.c \
	error.c \
	list.c \
	analyze.c \
	conversion.c \
	symtab.c \
	codegen.c \
	main.c

all: $(PROGS)

tc: $(TC_SRC:.c=.o)
	$(LD) $(LDFLAGS) $(TC_SRC:.c=.o) -o tc -lfl

yy.lex.o: lex.yy.c
lex.yy.c: scan.l y.tab.h
	$(LEX) scan.l

y.tab.h: y.tab.c
y.tab.c: parse.y
	$(YACC) -d parse.y

y.output: parse.y
	$(YACC) -d -v parse.y

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

depend: scan.c y.tab.c
	makedepend *.c

clean:
	rm -f lex.yy.c y.tab.h y.tab.c
	rm -f $(PROGS)
	rm -f *.o *.do
	rm -f Makefile.bak

.PHONY: clean depend
# DO NOT DELETE
