CXX = g++
CXXFLAGS = -fsanitize=address -fno-omit-frame-pointer -pedantic -Wall -Wextra -g

interpreter: scanner.h scanner.cpp main.cpp
	$(CXX) $(CXXFLAGS) -o interpreter execution.h scanner.h execution.cpp scanner.cpp main.cpp

clean:
	rm -f *.o interpreter