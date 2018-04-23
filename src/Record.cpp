#include <sstream>
#include "Definitions.hpp"
#include "Record.hpp"

Record::Record( ValueType value )
    : m_value( value )
{

}

ValueType Record::value() const
{
    return m_value;
}

std::string Record::to_string() const
{
    std::ostringstream oss;
    oss << m_value;
    return oss.str();
}
