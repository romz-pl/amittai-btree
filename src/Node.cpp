#include "Node.hpp"

//
//
//
Node::Node( std::uint32_t order, InternalNode* parent )
    : m_order{ order }
    , m_parent{ parent }
{

}

//
//
//
std::uint32_t Node::order() const
{
    return m_order;
}

//
//
//
InternalNode* Node::parent() const
{
    return m_parent;
}

//
//
//
void Node::set_parent( InternalNode *parent )
{
    m_parent = parent;
}

//
//
//
bool Node::is_root() const
{
    return !m_parent;
}
