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
Node::Node( int order, Node* parent )
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
Node* Node::parent() const
{
    return m_parent;
}

//
//
//
void Node::set_parent( Node* parent )
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
