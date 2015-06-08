# A sample Makefile for building Google Test and using it in user
# tests.  Please tweak it to suit your environment and project.  You
# may want to move it to your project's root directory.
#
# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make TARGET - makes the given target.
#   make clean  - removes all files generated by make.

# Please tweak the following variable definitions as needed by your
# project, except GTEST_HEADERS, which you can use in your own targets
# but shouldn't modify.

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../gtest-1.7.0

# Where to find user code.
USER_DIR = ./source

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -lncurses -std=c++11

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

TESTS = Zombie_unittest
BIN = Zombies
SOURCE_FILES = $(USER_DIR)/main.cpp $(USER_DIR)/Simulation.cpp $(USER_DIR)/Simulation.h \
				$(USER_DIR)/Corpses.cpp $(USER_DIR)/Corpses.h \
				$(USER_DIR)/Zombie.cpp $(USER_DIR)/Zombie.h \
				$(USER_DIR)/ZombieFabric.cpp $(USER_DIR)/ZombieFabric.h \

# House-keeping build targets.
all : test build

build : 
	$(CXX) -o $(BIN) $(USER_DIR)/*.cpp $(USER_DIR)/*.h  $(CXXFLAGS)

build_custom :
	$(CXX) -o $(BIN) $(SOURCE_FILES) $(CXXFLAGS)

test : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o
	rm $(BIN)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

#sample1.o : $(USER_DIR)/sample1.cc $(USER_DIR)/sample1.h $(GTEST_HEADERS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1.cc
#
#sample1_unittest.o : $(USER_DIR)/sample1_unittest.cc \
#                     $(USER_DIR)/sample1.h $(GTEST_HEADERS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1_unittest.cc
#
#sample1_unittest : sample1.o sample1_unittest.o gtest_main.a
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@


Zombie.o : $(USER_DIR)/Zombie.cpp $(USER_DIR)/Zombie.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/Zombie.cpp

Zombie_unittest.o : $(USER_DIR)/Zombie_unittest.cc \
                     $(USER_DIR)/Zombie.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/Zombie_unittest.cc

Zombie_unittest : Zombie.o Zombie_unittest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@







