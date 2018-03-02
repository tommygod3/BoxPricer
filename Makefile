CC := g++

SRCDIR := src
BUILDDIR := build
TARGET := bin/executable

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CXXFLAGS := -std=c++11 -Wall -I include -g
DBOOST := -lboost_unit_test_framework
LIB := 

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) -r $(TARGET) $(BUILDDIR)/*

#Tests
tester:
	$(CC) $(CXXFLAGS) test/test.cpp $(DBOOST) -o bin/test

.PHONY: clean
