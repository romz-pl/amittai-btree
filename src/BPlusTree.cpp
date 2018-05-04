#include <stdexcept>
#include <algorithm>
#include <cassert>
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
        delete m_root->leaf();
    }
    else
    {
        delete m_root->internal();
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
Record* BPlusTree::search( const KeyType& key )
{
    if( is_empty() )
    {
        return nullptr;
    }

    LeafNode* leaf_node = find_leaf_node( key );
    assert( leaf_node );

    return leaf_node->lookup( key );
}

//
//
//
LeafNode* BPlusTree::find_leaf_node( const KeyType& key )
{
    assert( !is_empty() );

    auto node = m_root;
    while( !node->is_leaf() )
    {
        InternalNode* internalNode = node->internal();
        node = internalNode->lookup( key );
    }

    return node->leaf();
}


//
// INSERTION
//

//
//
//
void BPlusTree::insert( const KeyType& key, ValueType value )
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
void BPlusTree::start_new_tree( const KeyType& key, ValueType value )
{
    LeafNode* new_leaf_node = new LeafNode( this, nullptr );
    new_leaf_node->create_and_insert_record( key, value );
    m_root = new_leaf_node;
}

//
//
//
void BPlusTree::insert_into_leaf( const KeyType& key, ValueType value )
{
    LeafNode* leaf_node = find_leaf_node( key );
    assert( leaf_node );

    leaf_node->create_and_insert_record( key, value );
    if( leaf_node->size() > leaf_max_size() )
    {
        LeafNode* new_leaf = split( leaf_node );
        new_leaf->set_next( leaf_node->next() );
        leaf_node->set_next( new_leaf );
        const KeyType newKey = new_leaf->first_key();
        insert_into_parent( leaf_node, newKey, new_leaf );
    }
}

//
//
//
void BPlusTree::insert_into_parent(  Node *old_node, const KeyType& key, Node *new_node )
{
    if( old_node->is_root() )
    {
        InternalNode* new_root = new InternalNode( this, nullptr );
        old_node->set_parent( new_root );
        new_node->set_parent( new_root );
        new_root->populate_new_root( old_node, key, new_node );
        m_root = new_root;
    }
    else
    {
        InternalNode* parent = old_node->get_parent();
        assert( parent );
        parent->insert_node_after( old_node, key, new_node );
        if( parent->size() > internal_max_size() )
        {
            InternalNode* new_node = split( parent );
            const KeyType new_key = new_node->replace_and_return_first_key();
            insert_into_parent( parent, new_key, new_node );
        }
    }
}

//
//
//
LeafNode* BPlusTree::split( LeafNode* node )
{
    LeafNode* new_node = new LeafNode( this, node->get_parent() );
    node->move_half_to( new_node );
    return new_node;
}

//
//
//
InternalNode* BPlusTree::split( InternalNode* node )
{
    InternalNode* new_node = new InternalNode( this, node->get_parent() );
    node->move_half_to( new_node );
    return new_node;
}

//
// REMOVAL
//

//
//
//
void BPlusTree::remove( const KeyType& key )
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
void BPlusTree::remove_from_leaf( const KeyType& key )
{
    LeafNode* leafNode = find_leaf_node( key );
    assert( leafNode );

    if( !leafNode->lookup( key ) )
    {
        return;
    }

    const std::size_t new_size = leafNode->remove_and_delete_record( key );
    if( new_size < leaf_min_size() )
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
    InternalNode* parent = node->get_parent();
    const std::size_t index_of_node_in_parent = parent->node_index( node );
    const std::size_t neighbor_index = ( index_of_node_in_parent == 0 ) ? 1 : index_of_node_in_parent - 1;
    LeafNode* neighbor_node = parent->neighbor( neighbor_index )->leaf();

    if( node->size() + neighbor_node->size() <= leaf_max_size() )
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

    InternalNode* parent = node->get_parent();
    const std::size_t index_of_node_in_parent = parent->node_index( node );
    const std::size_t neighbor_index = ( index_of_node_in_parent == 0 ) ? 1 : index_of_node_in_parent - 1;
    InternalNode* neighbor_node = parent->neighbor( neighbor_index )->internal();

    if( node->size() + neighbor_node->size() <= internal_max_size() )
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

    if( parent->size() < internal_min_size() )
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

    if( parent->size() < internal_min_size() )
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
        auto discarded_node = m_root->internal();
        m_root = m_root->internal()->remove_and_return_only_child();
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
std::size_t BPlusTree::leaf_min_size() const
{
    return m_order / 2;
}

//
//
//
std::size_t BPlusTree::leaf_max_size() const
{
    return m_order - 1;
}

//
//
//
std::size_t BPlusTree::internal_min_size() const
{
    return m_order / 2;
}

//
//
//
std::size_t BPlusTree::internal_max_size() const
{
    // Includes the first entry, which
    // has key DUMMY_KEY and a value that
    // points to the left of the first positive key k1
    // (i.e., a node whose keys are all < k1).
    return m_order;
}

