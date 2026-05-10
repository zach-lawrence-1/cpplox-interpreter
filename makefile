CXX = g++
CXXFLAGS = -fsanitize=address -fno-omit-frame-pointer -pedantic -Wall -Wextra -g
INCLUDES = -Iexternal/googletest/googletest/include -Iexternal/googletest/googletest
FILES = execution.cpp ast.cpp astPrinter.cpp interpreterObject.cpp scanner.cpp parser.cpp
TEST_FILES = tests/test_scanner.cpp tests/test_parser.cpp
GTEST_FILES = external/googletest/googletest/src/gtest-all.cc external/googletest/googletest/src/gtest_main.cc

interpreter: $(FILES)
	$(CXX) $(CXXFLAGS) -o interpreter $(FILES) main.cpp

tests: $(TEST_FILES) $(GTEST_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_FILES) $(FILES) $(GTEST_FILES) -pthread -o runTests 

clean:
	rm -f *.o interpreter runTests