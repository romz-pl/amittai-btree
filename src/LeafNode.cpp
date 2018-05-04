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
void LeafNode::insert( const KeyType& key, ValueType value )
{
    assert( is_sorted() );

    const auto pred = [ key ]( const LeafElt& m ){ return m.m_key >= key; };
    const auto it = std::find_if( m_elt.begin(), m_elt.end(), pred );

    if( it != m_elt.end() && it->m_key == key )
    {
        throw std::runtime_error( "Key duplication" );
    }

    Record* record = new Record( value );
    m_elt.insert( it, LeafElt( key, record ) );

    assert( is_sorted() );
}


//
//
//
Record* LeafNode::lookup( const KeyType& key ) const
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
void LeafNode::remove( const KeyType& key )
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
void LeafNode::move_half( LeafNode *from, LeafNode *to )
{
    assert( from->m_tree == to->m_tree );

    const auto m = from->m_elt.begin() + from->m_tree->leaf_min_size();
    const auto e = from->m_elt.end();

    to->m_elt.insert( to->m_elt.end(), std::make_move_iterator( m ), std::make_move_iterator( e ) );

    from->m_elt.erase( m, e );
}

//
//
//
void LeafNode::move_all( LeafNode *from, LeafNode *to )
{
    const auto b = from->m_elt.begin();
    const auto e = from->m_elt.end();

    to->m_elt.insert( to->m_elt.end(), std::make_move_iterator( b ), std::make_move_iterator( e ) );

    from->m_elt.clear( );

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

