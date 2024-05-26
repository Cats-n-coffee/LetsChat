CC = g++ -std=c++11
OUT = ../out/main

ODIR = obj

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp
	$(CC) -c -o $@ $<

$(OUT): $(OBJ)
	$(CC) -o $@ $^