#define ENVIRONMENT
#ifdef ENVIRONMENT

#include <unordered_map>
#include "scanner.h"
#include "interpreterObject.h"

class Environment
{
    private:
        std::unordered_map<std::string, InterpreterObject> variables;

    public:
        Environment() = default;
        InterpreterObject get(Token name);
        void define(Token name, InterpreterObject value);
};

#endif