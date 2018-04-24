#include <cassert>
#include "Node.hpp"

//
//
//
Node::Node( std::size_t order, InternalNode* parent )
    : m_order{ order }
    , m_parent{ parent }
{

}

//
//
//
std::size_t Node::order() const
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

//
//
//
InternalNode* Node::internal()
{
    assert( 0 );
    return nullptr;
}

//
//
//
const InternalNode* Node::internal() const
{
    assert( 0 );
    return nullptr;
}

//
//
//
LeafNode* Node::leaf()
{
    assert( 0 );
    return nullptr;
}

//
//
//
const LeafNode* Node::leaf() const
{
    assert( 0 );
    return nullptr;
}
