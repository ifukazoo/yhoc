PROGRAM    := hoc
OBJS       := lexer.o hoc.o symbol.o

all        : $(PROGRAM)
$(OBJS)    : y.tab.h
y.tab.h    : parser.o
$(PROGRAM) : parser.o $(OBJS) builtin.o
		$(LINK.o) $^ $(LDLIBS) -o $@


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
