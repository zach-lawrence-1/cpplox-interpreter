CXX = g++
CXXFLAGS = -fsanitize=address -fno-omit-frame-pointer -pedantic -Wall -Wextra -g
FILES = execution.cpp ast.cpp astPrinter.cpp interpreterObject.cpp scanner.cpp parser.cpp main.cpp

interpreter: $(FILES)
	$(CXX) $(CXXFLAGS) -o interpreter $(FILES)

clean:
	rm -f *.o interpreter