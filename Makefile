PROGRAM    := hoc

$(PROGRAM) : parser.o lexer.o hoc.o
		    $(LINK.o) $^ $(LDLIBS) -o $@

hoc.o   : y.tab.h
y.tab.h : parser.o

all:   $(PROGRAM)

YACC = bison
LEX = flex
CC = gcc
YFLAGS = -dy -v -t
CFLAGS = -g -Wall -Wextra -MMD -MP
LDLIBS = -lfl -lm

-include *.d
.PHONY: clean
clean:
		rm -rf *.o *.d parser.[oc] lexer.[oc] y.* $(PROGRAM)
