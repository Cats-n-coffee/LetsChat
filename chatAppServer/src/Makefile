CC = g++
OUT = ../out/main

ODIR = obj

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp
	$(CC) -c -o $@ $<

$(OUT): $(OBJ)
	$(CC) -o $@ $^