#include <sstream>
#include <string>
#include "Exceptions.hpp"
#include "Node.hpp"

LeafNotFoundException::LeafNotFoundException( KeyType key )
    : m_key{ key }
{

}

const char* LeafNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Key not found in any leaf node:  ";
    ss << m_key;
    static std::string message;
    message = ss.str();
    return message.c_str();
}


NodeNotFoundException::NodeNotFoundException( std::string searched_node, std::string containing_node )
    : m_searched_node{ searched_node }
    , m_containing_node{ containing_node }
{

}

const char* NodeNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Node |" << m_searched_node << "| not found";
    ss << " as a child of node ";
    ss << m_containing_node;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

RecordNotFoundException::RecordNotFoundException( KeyType key )
    : m_key{ key }
{

}

const char* RecordNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Record not found with key:  " << m_key;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

