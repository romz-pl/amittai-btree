#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "LeafNode.hpp"
#include "InternalNode.hpp"


//
//
//
LeafNode::LeafNode( BPlusTree *tree, InternalNode* parent )
    : Node( tree, parent )
    , m_next{ nullptr }
{

}

//
//
//
LeafNode::~LeafNode()
{
    for( auto m : m_elt )
    {
        delete m.m_record;
    }
}

//
//
//
bool LeafNode::is_leaf() const
{
    return true;
}

//
//
//
std::size_t LeafNode::size() const
{
    return m_elt.size();
}


//
//
//
LeafNode* LeafNode::next() const
{
    return m_next;
}

//
//
//
void LeafNode::set_next( LeafNode* next )
{
    m_next = next;
}

//
//
//
std::size_t LeafNode::create_and_insert_record( KeyType key, ValueType value )
{
    assert( is_sorted() );

    Record* existing_record = lookup( key );
    if( !existing_record )
    {
        Record* new_record = new Record( value );
        insert( key, new_record );
    }
    assert( is_sorted() );
    return m_elt.size();
}

//
//
//
void LeafNode::insert( KeyType key, Record* record )
{
    assert( is_sorted() );

    const auto pred = [ key ]( const LeafElt& m ){ return m.m_key >= key; };
    const auto insertion_point = std::find_if( m_elt.begin(), m_elt.end(), pred );
    m_elt.insert( insertion_point, LeafElt( key, record ) );

    assert( is_sorted() );
}

//
//
//
Record* LeafNode::lookup( KeyType key ) const
{
    const auto pred = [ key ]( const LeafElt& m ){ return m.m_key == key; };
    const auto it = std::find_if( m_elt.begin(), m_elt.end(), pred );

    if( it != m_elt.end() )
    {
        return it->m_record;
    }

    return nullptr;
}

//
//
//
std::size_t LeafNode::remove_and_delete_record( KeyType key )
{
    assert( is_sorted() );

    const auto pred = [ key ]( const LeafElt& m ){ return m.m_key == key; };
    const auto removal_point = std::find_if( m_elt.begin(), m_elt.end(), pred );

    if( removal_point == m_elt.end() )
    {
        throw std::runtime_error( "Key Not Found" );
    }

    delete removal_point->m_record;
    m_elt.erase( removal_point );

    assert( is_sorted() );

    return m_elt.size();
}

//
//
//
KeyType LeafNode::first_key() const
{
    assert( !m_elt.empty() );
    return m_elt[ 0 ].m_key;
}

//
//
//
void LeafNode::move_half_to( LeafNode *recipient )
{
    assert( is_sorted() );

    recipient->copy_half_from( m_elt );
    const auto beg = m_elt.begin() + m_tree->leaf_min_size();
    m_elt.erase( beg, m_elt.end() );

    assert( is_sorted() );
}

//
//
//
void LeafNode::copy_half_from(const std::vector< LeafElt > &ve )
{
    assert( is_sorted() );

    m_elt.reserve( m_elt.size() + ve.size() - m_tree->leaf_min_size() );
    const auto beg = ve.begin() + m_tree->leaf_min_size();
    m_elt.insert( m_elt.end(), beg, ve.end() );

    assert( is_sorted() );
}

//
//
//
void LeafNode::move_all_to( LeafNode *recipient )
{
    assert( is_sorted() );

    recipient->copy_all_from( m_elt );
    m_elt.clear();
    recipient->set_next( next() );

    assert( is_sorted() );
}

//
//
//
void LeafNode::copy_all_from(const std::vector< LeafElt > &ve )
{
    assert( is_sorted() );

    m_elt.reserve( m_elt.size() + ve.size() );
    m_elt.insert( m_elt.end(), ve.begin(), ve.end() );

    assert( is_sorted() );
}

//
//
//
void LeafNode::move_first_to_end_of( LeafNode* recipient )
{
    assert( is_sorted() );

    recipient->copy_last_from( m_elt.front() );
    m_elt.erase( m_elt.begin() );
    get_parent()->set_key_at( 1, m_elt.front().m_key );

    assert( is_sorted() );
}

//
//
//
void LeafNode::copy_last_from( const LeafElt& pair )
{
    assert( is_sorted() );

    m_elt.push_back( pair );

    assert( is_sorted() );
}

//
//
//
void LeafNode::move_last_to_front_of( LeafNode *recipient, std::size_t parent_index )
{
    assert( is_sorted() );

    recipient->copy_first_from( m_elt.back(), parent_index );
    m_elt.pop_back();

    assert( is_sorted() );
}

//
//
//
void LeafNode::copy_first_from( const LeafElt& pair, std::size_t parent_index )
{
    assert( is_sorted() );

    m_elt.insert( m_elt.begin(), pair );
    get_parent()->set_key_at( parent_index, m_elt.front().m_key );

    assert( is_sorted() );
}

//
//
//
bool LeafNode::is_sorted() const
{
    const auto pred = []( const LeafElt& ma, const LeafElt& mb ){ return ma.m_key < mb.m_key ; };

    return std::is_sorted( m_elt.begin(), m_elt.end(), pred );
}

//
//
//
LeafNode* LeafNode::leaf()
{
    return this;
}

//
//
//
const LeafNode* LeafNode::leaf() const
{
    return this;
}

