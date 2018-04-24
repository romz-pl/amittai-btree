#include <algorithm>
#include <cassert>
#include <iterator>
#include "InternalNode.hpp"



//
//
//
InternalNode::InternalNode( std::size_t order, InternalNode *parent )
    : Node( order, parent )
{

}

//
//
//
InternalNode::~InternalNode()
{
    for( auto mapping : m_mappings )
    {
        delete mapping.m_node;
    }
}

//
//
//
bool InternalNode::is_leaf() const
{
    return false;
}

//
//
//
std::size_t InternalNode::size() const
{
    return m_mappings.size();
}

//
//
//
std::size_t InternalNode::min_size() const
{
    return order() / 2;
}

//
//
//
std::size_t InternalNode::max_size() const
{
    // Includes the first entry, which
    // has key DUMMY_KEY and a value that
    // points to the left of the first positive key k1
    // (i.e., a node whose keys are all < k1).
    return order();
}

//
//
//
KeyType InternalNode::key_at( std::size_t index ) const
{
    assert( index < m_mappings.size() );
    return m_mappings[ index ].m_key;
}

//
//
//
void InternalNode::set_key_at( std::size_t index, KeyType key )
{
    assert( index < m_mappings.size() );
    m_mappings[ index ].m_key = key;
}

//
//
//
Node* InternalNode::first_child() const
{
    assert( !m_mappings.empty() );
    return m_mappings.front().m_node;
}

//
//
//
void InternalNode::populate_new_root( Node *old_node, KeyType new_key, Node *new_node )
{
    // assert( is_sorted() );

    m_mappings.push_back( InternalElt( DUMMY_KEY, old_node ) );
    m_mappings.push_back( InternalElt( new_key, new_node ) );

    // assert( is_sorted() );
}

//
//
//
std::size_t InternalNode::insert_node_after( Node *old_node, KeyType new_key, Node *new_node )
{
    // assert( is_sorted() );

    const auto pred = [ old_node ]( const InternalElt& e ){ return e.m_node == old_node; };
    const auto iter = std::find_if( m_mappings.begin(), m_mappings.end(), pred );
    assert( iter != m_mappings.end() );

    m_mappings.insert( iter + 1, InternalElt( new_key, new_node ) );

    // assert( is_sorted() );
    return size();
}

//
//
//
void InternalNode::remove( std::size_t index )
{
    // assert( is_sorted() );

    m_mappings.erase( m_mappings.begin() + index );

    // assert( is_sorted() );
}

//
//
//
Node* InternalNode::remove_and_return_only_child()
{
    // assert( is_sorted() );

    Node* first_child = m_mappings.front().m_node;
    m_mappings.pop_back();

    // assert( is_sorted() );

    return first_child;
}

//
//
//
KeyType InternalNode::replace_and_return_first_key()
{
    // assert( is_sorted() );

    KeyType new_key = m_mappings[ 0 ].m_key;
    m_mappings[ 0 ].m_key = KeyType( DUMMY_KEY );

    // assert( is_sorted() );
    return new_key;
}

//
//
//
void InternalNode::move_half_to( InternalNode *recipient )
{
    // assert( is_sorted() );

    recipient->copy_half_from( m_mappings );
    const std::size_t size = m_mappings.size();
    for( std::size_t i = min_size(); i < size; ++i )
    {
        m_mappings.pop_back();
    }

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_half_from( const std::vector< InternalElt >& mapp )
{
    // assert( is_sorted() );

    for( std::size_t i = min_size(); i < mapp.size(); ++i )
    {
        mapp[ i ].m_node->set_parent( this );
        m_mappings.push_back( mapp[ i ] );
    }

    // assert( is_sorted() );
}

//
//
//
void InternalNode::move_all_to( InternalNode *recipient, std::size_t index_in_parent )
{
    // assert( is_sorted() );

    m_mappings[ 0 ].m_key = parent()->key_at( index_in_parent );
    recipient->copy_all_from( m_mappings );
    m_mappings.clear();

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_all_from( const std::vector< InternalElt > &mapp )
{
    // assert( is_sorted() );

    for( auto m : mapp )
    {
        m.m_node->set_parent( this );
        m_mappings.push_back( m );
    }

    // assert( is_sorted() );
}

//
//
//
void InternalNode::move_first_to_end_of( InternalNode *recipient )
{
    // assert( is_sorted() );

    recipient->copy_last_from( m_mappings.front() );
    m_mappings.erase( m_mappings.begin() );
    parent()->set_key_at( 1, m_mappings.front().m_key );

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_last_from( const InternalElt& pair )
{
    // assert( is_sorted() );

    m_mappings.push_back( pair );
    m_mappings.back().m_node->set_parent( this );

    // assert( is_sorted() );
}

//
//
//
void InternalNode::move_last_to_front_of( InternalNode *recipient, std::size_t parent_index )
{
    // assert( is_sorted() );

    recipient->copy_first_from( m_mappings.back(), parent_index );
    m_mappings.pop_back();

    // assert( is_sorted() );
}

//
//
//
void InternalNode::copy_first_from( const InternalElt& pair, std::size_t parent_index )
{
    // assert( is_sorted() );

    m_mappings.front().m_key = parent()->key_at( parent_index );
    m_mappings.insert( m_mappings.begin(), pair );
    m_mappings.front().m_key = KeyType( DUMMY_KEY );
    m_mappings.front().m_node->set_parent( this );
    parent()->set_key_at( parent_index, m_mappings.front().m_key );

    // assert( is_sorted() );
}

//
//
//
Node* InternalNode::lookup( KeyType key ) const
{
/*    // assert( is_sorted() );
    if( !is_sorted() )
    {
        for( auto m : m_mappings )
        {
            std::cout << m.first.to_int64() << " " << std::flush;
        }
        std::cout << "\n";
        exit(1);
    }
*/
    auto locator = m_mappings.begin();
    auto end = m_mappings.end();
    while( locator != end && key >= locator->m_key )
    {
        ++locator;
    }
    // locator->first is now the least key k such that aKey < k.
    // One before is the greatest key k such that aKey >= k.
    --locator;
    return locator->m_node;
}

//
//
//
std::size_t InternalNode::node_index( Node *node ) const
{
    // assert( is_sorted() );

    const auto pred = [ node ]( const InternalElt& v ){ return( v.m_node == node ); };
    const auto ff = std::find_if( m_mappings.begin(), m_mappings.end(), pred );
    assert( ff != m_mappings.end() );

    const auto index = std::distance( m_mappings.begin(), ff );
    assert( index >= 0 );

    return static_cast< std::size_t >( index );
}

//
//
//
Node* InternalNode::neighbor( std::size_t index ) const
{
    // assert( is_sorted() );

    return m_mappings[ index ].m_node;
}

//
//
//
bool InternalNode::is_sorted() const
{
    const auto pred = []( const InternalElt& ma, const InternalElt& mb ){ return ma.m_key < mb.m_key ; };

    return std::is_sorted( m_mappings.begin(), m_mappings.end(), pred );
}


