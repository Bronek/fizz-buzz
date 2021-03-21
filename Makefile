
COMPILER  ?= clang++-12
CXXVER    ?= 2a
CXXDBG    ?= 0
CXXOPT    ?= 2
CXXFLAGS   = -c -Wall -Wextra -Wpedantic -Wno-type-limits -std=c++$(CXXVER) -O$(CXXOPT) -g$(CXXDBG)
LDFLAGS   ?=
SOURCES    = $(wildcard *.cpp)
OBJECTS    = $(SOURCES:.cpp=.o)
OUTPUT     = $(shell basename $(CURDIR))

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(COMPILER) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(COMPILER) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(OUTPUT)

