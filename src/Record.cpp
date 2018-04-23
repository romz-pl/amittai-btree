#include "Record.hpp"

Record::Record( ValueType value )
    : m_value( value )
{

}

ValueType Record::value() const
{
    return m_value;
}


