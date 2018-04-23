#ifndef ROMZ_AMITTAI_BTREE_EXCEPTIONS_H
#define ROMZ_AMITTAI_BTREE_EXCEPTIONS_H


#include <exception>
#include <string>
#include "Definitions.hpp"

class LeafNotFoundException : public std::exception
{
public:
    explicit LeafNotFoundException( KeyType key );
    const char* what() const noexcept override;
private:
    KeyType m_key;
};

class NodeNotFoundException : public std::exception
{
public:
    explicit NodeNotFoundException( std::string searched_node, std::string containing_node );
    const char* what() const noexcept override;
private:
    std::string m_searched_node;
    std::string m_containing_node;
};

class RecordNotFoundException : public std::exception
{
public:
    explicit RecordNotFoundException( KeyType key );
    const char* what() const noexcept override;
private:
    KeyType m_key;
};

#endif
