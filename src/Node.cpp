#include "Node.hpp"

//
//
//
Node::Node( int order )
    : m_order{ order }
    , m_parent{ nullptr }
{

}

//
//
//
Node::Node( int order, InternalNode* parent )
    : m_order{ order }
    , m_parent{ parent }
{

}

//
//
//
int Node::order() const
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
