PROGRAM = hoc
SRCS    = $(filter-out parser.c lexer.c, $(wildcard *.c))
OBJS    = $(SRCS:.c=.o) parser.o lexer.o
MAP     = $(PROGRAM).map

YACC    = bison
LEX     = flex
CC      = gcc
YFLAGS  = -dy -v -t
CFLAGS  = -g -Wall -Wextra -MMD -MP
LDLIBS  = -lfl -lm

all   : $(PROGRAM)
debug : $(PROGRAM)
debug : YFLAGS  += --report=all
debug : CFLAGS  += -D HOC_DEBUG
debug : LDFLAGS += -Wl,-Map=$(MAP)

$(PROGRAM) : $(OBJS)
builtin.o  : y.tab.h
y.tab.h    : parser.o

clean:
		rm -rf *.o *.d parser.[oc] lexer.[oc] y.* $(MAP) $(PROGRAM)

.PHONY: all clean
-include *.d
