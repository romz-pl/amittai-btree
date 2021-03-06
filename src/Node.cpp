#include <cassert>
#include "Node.hpp"

//
//
//
Node::Node( BPlusTree *tree, InternalNode* parent )
    : m_tree{ tree }
    , m_parent{ parent }
{
    assert( m_tree );
}

//
//
//
InternalNode *Node::get_parent()
{
    return m_parent;
}

/*
//
//
//
const InternalNode *Node::get_parent() const
{
    return m_parent;
}
*/

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
