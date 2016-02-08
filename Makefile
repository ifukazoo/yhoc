PROGRAM    := hoc
OBJS       := \
							lexer.o\
							hoc.o\
							symbol.o\
							builtin.o\
							code.o\

all        : $(PROGRAM)
debug      : $(PROGRAM)
$(OBJS)    : y.tab.h
y.tab.h    : parser.o
$(PROGRAM) : parser.o $(OBJS)
		$(LINK.o) $^ $(LDLIBS) -o $@


YACC = bison
LEX = flex
CC = gcc
YFLAGS = -dy -v -t
CFLAGS = -g -Wall -Wextra -MMD -MP
LDLIBS = -lfl -lm

debug : YFLAGS += -l
debug : CFLAGS += -D HOC_DEBUG

-include *.d
.PHONY: clean
clean:
		rm -rf *.o *.d parser.[oc] lexer.[oc] y.* $(PROGRAM)
