CXX = g++
CXXFLAGS = -fno-omit-frame-pointer -pedantic -Wall -Wextra -g

generateAST: generateASTClasses.cpp
	$(CXX) $(CXXFLAGS) -o generateAST generateASTClasses.cpp

clean:
	rm -f *.o generateAST