#ifndef INTOBJ
#define INTOBJ

#include <string>

enum ObjectType
{
    STRING, BOOL, NUMBER, NIL
};

class InterpreterObject
{
    private:
        double m_number;
        bool m_boolean;
        std::string m_string;
        ObjectType m_type;

    public:
        InterpreterObject();
        InterpreterObject(double number);
        InterpreterObject(bool boolean);
        InterpreterObject(const std::string& strings);
        InterpreterObject(const char* cstr);
        InterpreterObject(int number);

        ObjectType getType();
        double getNumberValue();
        bool getBoolValue();
        std::string getStringValue();
};

#endif