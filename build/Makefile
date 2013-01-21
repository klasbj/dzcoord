
CXX := g++
CXXFLAGS := -Wall -g -std=c++11

VPATH+=$(CURDIR)/../src 

OBJS := parser.o mux.o area.o

BINDIR := $(CURDIR)/../bin
BIN := $(BINDIR)/mux

all: $(BIN)

parser.o: area.h parser.h

mux.o: area.h parser.h

area.o: area.h

$(BINDIR):
	mkdir -p $(BINDIR)

$(BIN): $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

.o.cpp:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
