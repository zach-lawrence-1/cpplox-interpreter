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
        std::string m_string;
        double m_number;
        ObjectType m_type;
        bool m_boolean;

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
        bool operator==(const InterpreterObject& right) const;
};

std::string getObjectValueAsString(InterpreterObject obj);

#endif