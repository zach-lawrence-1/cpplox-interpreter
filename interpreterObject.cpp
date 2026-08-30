#include "interpreterObject.h"

InterpreterObject::InterpreterObject()
{
    m_number = 0;
    m_string = "";
    m_boolean = false;
    m_type = NIL;
}

InterpreterObject::InterpreterObject(double number)
{
    m_number = number;
    m_type = NUMBER;
}

InterpreterObject::InterpreterObject(bool boolean)
{
    m_boolean = boolean;
    m_type = BOOL;
}

InterpreterObject::InterpreterObject(const char* cstr)
{
    m_string = std::string(cstr);
    m_type = STRING;
}

InterpreterObject::InterpreterObject(const std::string& strings)
{
    m_string = strings;
    m_type = STRING;
}

InterpreterObject::InterpreterObject(int number)
{
    m_number = static_cast<double>(number);
    m_type = NUMBER;
}

ObjectType InterpreterObject::getType()
{
    return m_type;
}

double InterpreterObject::getNumberValue()
{
    return m_number;
}

bool InterpreterObject::getBoolValue()
{
    return m_boolean;
}

std::string InterpreterObject::getStringValue()
{
    return m_string;
}

bool InterpreterObject::operator==(const InterpreterObject& right) const
{
    if (this->m_type != right.m_type)
        return false;
    
    switch (this->m_type)
    {
        case NUMBER:
            if (this->m_number != right.m_number)
                return false;
            break;
        case STRING:
            if (this->m_string != right.m_string)
                return false;
            break;
        case BOOL:
            if (this->m_boolean != right.m_boolean)
                return false;
            break;
        case NIL:
            break;
    }

    return true;
}

std::string getObjectValueAsString(InterpreterObject obj)
{
    switch(obj.getType())
    {
        case STRING:
            return obj.getStringValue();
        case BOOL:
        {
            if (obj.getBoolValue())
                return "true";
            return "false";
        }
        case NUMBER:
            return std::to_string(obj.getNumberValue());
        case NIL:
            return "nil";
    }

    return "";
}
