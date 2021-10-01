CC = gcc
CFLAGS = -O0 -std=c99 -Wall
#CFLAGS = -Og -std=c99 -Wall
DEPS = alu.h
OBJ = integerlab.o integergrader.o alu.o
CHECK = nl alu.s | grep -e "addb" -e "addw" -e "addl" -e "addq" -e "subb" -e "subw" -e "subl" -e "subq" -e "imulb" -e "imulw" -e "imull" -e "imulq" -e "divb" -e "divw" -e "divl" -e "divq" -e "incb" -e "incw" -e "incl" -e "incq" -e "decb" -e "decw" -e "decl" -e "decq" -e "negb" -e "negw" -e "negl" -e "negq" | grep -v "rsp" | grep -v "\." | grep -v "call" | grep -v "mov"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

integerlab: integerlab.o alu.o
	$(CC) -o $@ $^ $(CFLAGS)

integergrader: integergrader.o alu.o
	$(CC) -o $@ $^ $(CFLAGS)

assembly: alu.c $(DEPS)
	$(CC) -S $< $(CFLAGS)

.PHONY: clean clear all operatorcheck

operatorcheck : assembly
	$(CHECK)

all: integerlab integergrader assembly

clean:
	rm -f $(OBJ) *.s *~ core

clear: clean
	rm integerlab

