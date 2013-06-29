
CXX := g++
CXXFLAGS := -Wall -g -std=c++11

ifeq ($(DEBUG),1)
  CXXFLAGS += -DDEBUG
endif

VPATH+=$(CURDIR)/../src 

OBJS := parser.o mux.o area.o options.o dzen.o screen.o

BINDIR := $(CURDIR)/../bin
BIN := $(BINDIR)/dzcoord

all: $(BIN)

release: $(BIN)

parser.o: area.h parser.h options.h

mux.o: area.h parser.h options.h

area.o: area.h

options.o: options.h

$(BINDIR):
	mkdir -p $(BINDIR)

$(BIN): $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(BIN)
