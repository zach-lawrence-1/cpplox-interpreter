# cpplox-interpreter
## About the project
Making an interpreter for the lox language in C++ utilizing the crafting interpreters book as a reference. I wanted to learn how interpreters and compilers work under the hood and thought this would be a perfect project.

## How To Run
### Downloading The Code
Download the code and make file as follows
Git:
1. ```git clone https://github.com/zach-lawrence-1/cpplox-interpreter.git c```
   - c can be any local directory you want.
2. ```cd c```

OR

Just download zip from github and unzip where you want
### Windows
On windows, as long as you have MSYS2 setup with g++ and you can run make files then you should be able to run the main program as follows:
1. Download code and make file.
2. Open a terminal in the same directory as the code and make file.
3. Remove ```-fsanitize=address``` from ```CXXFLAGS``` variable in makefile since it doesn't seem to work with MSYS2 on windows
4. Type in ```make``` or ```make interpreter```
5. run ```./interpreter.exe``` or ```./interpreter```
6. You can also pass in files with ```./interpreter.exe filename.cpplox``` or ```./interpreter filename.cpplox```

Will work on linux without any modification to makefile