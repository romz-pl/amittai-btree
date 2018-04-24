#include <stdexcept>
#include <algorithm>
#include "BPlusTree.hpp"
#include "InternalNode.hpp"
#include "LeafNode.hpp"
#include "Node.hpp"

//
// Minimum order is necessarily 3.
//
BPlusTree::BPlusTree( std::size_t order )
    : m_order{ std::max( order, static_cast< std::size_t >( 3 ) ) }
    , m_root{ nullptr }
{

}

//
//
//
BPlusTree::~BPlusTree()
{
    destroy_tree();
}

//
//
//
void BPlusTree::destroy_tree()
{
    if( !m_root )
    {
        return;
    }

    if( m_root->is_leaf() )
    {
        delete static_cast< LeafNode* >( m_root );
    }
    else
    {
        delete static_cast< InternalNode* >( m_root );
    }
    m_root = nullptr;
}

//
//
//
bool BPlusTree::is_empty() const
{
    return !m_root;
}

//
//
//
Record* BPlusTree::search( KeyType key )
{
    LeafNode* leafNode = find_leaf_node( key );
    if( !leafNode )
    {
        return nullptr;
    }
    return leafNode->lookup( key );
}


//
// INSERTION
//

//
//
//
void BPlusTree::insert( KeyType key, ValueType value )
{
    if( is_empty() )
    {
        start_new_tree( key, value );
    }
    else
    {
        insert_into_leaf( key, value );
    }
}

//
//
//
void BPlusTree::start_new_tree( KeyType key, ValueType value )
{
    LeafNode* new_leaf_node = new LeafNode( m_order, nullptr );
    new_leaf_node->create_and_insert_record( key, value );
    m_root = new_leaf_node;
}

//
//
//
void BPlusTree::insert_into_leaf( KeyType key, ValueType value )
{
    LeafNode* leafNode = find_leaf_node( key );
    if( !leafNode )
    {
        throw std::runtime_error( "Leaf Not Found" );
    }

    const std::size_t newSize = leafNode->create_and_insert_record( key, value );
    if( newSize > leafNode->max_size() )
    {
        LeafNode* newLeaf = split( leafNode );
        newLeaf->set_next( leafNode->next() );
        leafNode->set_next( newLeaf );
        KeyType newKey = newLeaf->first_key();
        insert_into_parent( leafNode, newKey, newLeaf );
    }
}

//
//
//
void BPlusTree::insert_into_parent(  Node *old_node, KeyType key, Node *new_node )
{
    InternalNode* parent = old_node->parent();
    if (parent == nullptr)
    {
        m_root = new InternalNode( m_order, nullptr );
        parent = static_cast< InternalNode* >( m_root );
        old_node->set_parent( parent );
        new_node->set_parent( parent );
        parent->populate_new_root( old_node, key, new_node );
    }
    else
    {
        const std::size_t newSize = parent->insert_node_after( old_node, key, new_node );
        if( newSize > parent->max_size() )
        {
            InternalNode* newNode = split( parent );
            KeyType newKey = newNode->replace_and_return_first_key();
            insert_into_parent( parent, newKey, newNode );
        }
    }
}

//
//
//
LeafNode* BPlusTree::split( LeafNode* node )
{
    LeafNode* newNode = new LeafNode( m_order, node->parent() );
    node->move_half_to( newNode );
    return newNode;
}

//
//
//
InternalNode* BPlusTree::split( InternalNode* node )
{
    InternalNode* newNode = new InternalNode( m_order, node->parent() );
    node->move_half_to( newNode );
    return newNode;
}

//
// REMOVAL
//

//
//
//
void BPlusTree::remove( KeyType key )
{
    if( is_empty() )
    {
        return;
    }
    else
    {
        remove_from_leaf( key );
    }
}

//
//
//
void BPlusTree::remove_from_leaf( KeyType key )
{
    LeafNode* leafNode = find_leaf_node( key );
    if( !leafNode )
    {
        return;
    }

    if( !leafNode->lookup( key ) )
    {
        return;
    }

    const std::size_t newSize = leafNode->remove_and_delete_record( key );
    if( newSize < leafNode->min_size() )
    {
        coalesce_or_redistribute( leafNode );
    }
}

//
//
//
void BPlusTree::coalesce_or_redistribute( LeafNode* node )
{
    if( node->is_root() )
    {
        adjust_root();
        return;
    }
    InternalNode* parent = node->parent();
    const std::size_t index_of_node_in_parent = parent->node_index( node );
    const std::size_t neighbor_index = ( index_of_node_in_parent == 0 ) ? 1 : index_of_node_in_parent - 1;
    LeafNode* neighbor_node = static_cast< LeafNode* >( parent->neighbor( neighbor_index ) );

    if( node->size() + neighbor_node->size() <= neighbor_node->max_size() )
    {
        coalesce( neighbor_node, node, parent, index_of_node_in_parent );
    }
    else
    {
        redistribute( neighbor_node, node, index_of_node_in_parent );
    }
}

//
//
//
void BPlusTree::coalesce_or_redistribute( InternalNode* node )
{
    if( node->is_root() )
    {
        adjust_root();
        return;
    }

    InternalNode* parent = node->parent();
    const std::size_t index_of_node_in_parent = parent->node_index( node );
    const std::size_t neighbor_index = ( index_of_node_in_parent == 0 ) ? 1 : index_of_node_in_parent - 1;
    InternalNode* neighbor_node = static_cast< InternalNode* >( parent->neighbor( neighbor_index ) );

    if( node->size() + neighbor_node->size() <= neighbor_node->max_size() )
    {
        coalesce( neighbor_node, node, parent, index_of_node_in_parent );
    }
    else
    {
        redistribute( neighbor_node, node, index_of_node_in_parent );
    }
}

//
//
//
void BPlusTree::coalesce( LeafNode* neighbor_node, LeafNode* node, InternalNode* parent, std::size_t index )
{
    if( index == 0 )
    {
        std::swap( node, neighbor_node );
        index = 1;
    }

    node->move_all_to( neighbor_node );
    parent->remove( index );

    if( parent->size() < parent->min_size() )
    {
        coalesce_or_redistribute( parent );
    }
    delete node;
}

//
//
//
void BPlusTree::coalesce( InternalNode* neighbor_node, InternalNode* node, InternalNode* parent, std::size_t index )
{
    if( index == 0 )
    {
        std::swap( node, neighbor_node );
        index = 1;
    }

    node->move_all_to( neighbor_node, index );
    parent->remove( index );

    if( parent->size() < parent->min_size() )
    {
        coalesce_or_redistribute( parent );
    }
    delete node;
}

//
//
//
void BPlusTree::redistribute( LeafNode* neighbor_node, LeafNode* node, std::size_t index )
{
    if( index == 0 )
    {
        neighbor_node->move_first_to_end_of( node );
    }
    else
    {
        neighbor_node->move_last_to_front_of( node, index );
    }
}

//
//
//
void BPlusTree::redistribute( InternalNode* neighbor_node, InternalNode* node, std::size_t index )
{
    if ( index == 0 )
    {
        neighbor_node->move_first_to_end_of( node );
    }
    else
    {
        neighbor_node->move_last_to_front_of( node, index );
    }
}

//
//
//
void BPlusTree::adjust_root()
{
    if( !m_root->is_leaf() && m_root->size() == 1 )
    {
        auto discarded_node = static_cast< InternalNode* >( m_root );
        m_root = static_cast< InternalNode* >( m_root )->remove_and_return_only_child();
        m_root->set_parent( nullptr );
        delete discarded_node;
    }
    else if( !m_root->size() )
    {
        delete m_root;
        m_root = nullptr;
    }
}

//
//
//
LeafNode* BPlusTree::find_leaf_node( KeyType key )
{
    if( is_empty() )
    {
        return nullptr;
    }
    auto node = m_root;

    while( !node->is_leaf() )
    {
        auto internalNode = static_cast< InternalNode* >( node );
        node = internalNode->lookup( key );
    }
    return static_cast< LeafNode* >( node );
}

