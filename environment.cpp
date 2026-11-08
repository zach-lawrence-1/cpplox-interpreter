#include "environment.h"
#include "execution.h"

InterpreterObject Environment::get(Token name)
{
    if (variables.find(name.getLexeme()) != variables.end())
    {
        return variables[name.getLexeme()];
    }

    runtimeError(name, "undefined variable " + name.getLexeme());
    return InterpreterObject();
}

void Environment::define(Token name, InterpreterObject value)
{
    variables[name.getLexeme()] = value;
}