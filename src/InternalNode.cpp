#include <algorithm>
#include <cassert>
#include <iterator>
#include "InternalNode.hpp"




InternalNode::InternalNode( std::size_t order, InternalNode *parent )
    : Node( order, parent )
{

}

InternalNode::~InternalNode()
{
    for( auto mapping : m_mappings )
    {
        delete mapping.second;
    }
}

bool InternalNode::is_leaf() const
{
    return false;
}

std::size_t InternalNode::size() const
{
    return static_cast< std::size_t >( m_mappings.size() );
}

std::size_t InternalNode::min_size() const
{
    return order() / 2;
}

std::size_t InternalNode::max_size() const
{
    // Includes the first entry, which
    // has key DUMMY_KEY and a value that
    // points to the left of the first positive key k1
    // (i.e., a node whose keys are all < k1).
    return order();
}

KeyType InternalNode::key_at( std::size_t index ) const
{
    return m_mappings[ index ].first;
}

void InternalNode::set_key_at( std::size_t index, KeyType key )
{
    m_mappings[ index ].first = key;
}

Node* InternalNode::first_child() const
{
    return m_mappings.front().second;
}

void InternalNode::populate_new_root( Node *old_node, KeyType new_key, Node *new_node )
{
    // assert( is_sorted() );
    m_mappings.push_back( std::make_pair( DUMMY_KEY, old_node ) );
    m_mappings.push_back( std::make_pair( new_key, new_node ) );
    // assert( is_sorted() );
}

std::size_t InternalNode::insert_node_after( Node *old_node, KeyType new_key, Node *new_node )
{
    // assert( is_sorted() );

    auto iter = m_mappings.begin();
    for( ; iter != m_mappings.end() && iter->second != old_node; ++iter )
    {
        ; // empty instruction!
    }

    m_mappings.insert( iter + 1, std::make_pair( new_key, new_node ) );

    // assert( is_sorted() );
    return size();
}

void InternalNode::remove( std::size_t index )
{
    // assert( is_sorted() );
    m_mappings.erase( m_mappings.begin() + index );
    // assert( is_sorted() );
}

Node* InternalNode::remove_and_return_only_child()
{
    // assert( is_sorted() );
    Node* first_child = m_mappings.front().second;
    m_mappings.pop_back();
    // assert( is_sorted() );

    return first_child;
}

KeyType InternalNode::replace_and_return_first_key()
{
    // assert( is_sorted() );
    KeyType new_key = m_mappings[ 0 ].first;
    m_mappings[ 0 ].first = DUMMY_KEY;
    // assert( is_sorted() );
    return new_key;
}

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

void InternalNode::copy_half_from( std::vector< MappingType > &mappings )
{
    // assert( is_sorted() );
    for( std::size_t i = min_size(); i < mappings.size(); ++i )
    {
        mappings[ i ].second->set_parent( this );
        m_mappings.push_back( mappings[ i ] );
    }
    // assert( is_sorted() );
}

void InternalNode::move_all_to( InternalNode *recipient, std::size_t index_in_parent )
{
    // assert( is_sorted() );
    m_mappings[ 0 ].first = parent()->key_at( index_in_parent );
    recipient->copy_all_from( m_mappings );
    m_mappings.clear();
    // assert( is_sorted() );
}

void InternalNode::copy_all_from( std::vector< MappingType > &mappings )
{
    // assert( is_sorted() );
    for( auto m : mappings )
    {
        m.second->set_parent( this );
        m_mappings.push_back( m );
    }
    // assert( is_sorted() );
}

void InternalNode::move_first_to_end_of( InternalNode *recipient )
{
    // assert( is_sorted() );
    recipient->copy_last_from( m_mappings.front() );
    m_mappings.erase( m_mappings.begin() );
    parent()->set_key_at( 1, m_mappings.front().first );
    // assert( is_sorted() );
}

void InternalNode::copy_last_from( MappingType pair )
{
    // assert( is_sorted() );
    m_mappings.push_back( pair );
    m_mappings.back().second->set_parent( this );
    // assert( is_sorted() );
}

void InternalNode::move_last_to_front_of( InternalNode *recipient, std::size_t parent_index )
{
    // assert( is_sorted() );
    recipient->copy_first_from( m_mappings.back(), parent_index );
    m_mappings.pop_back();
    // assert( is_sorted() );
}

void InternalNode::copy_first_from( MappingType pair, std::size_t parent_index )
{
    // assert( is_sorted() );
    m_mappings.front().first = parent()->key_at( parent_index );
    m_mappings.insert( m_mappings.begin(), pair );
    m_mappings.front().first = DUMMY_KEY;
    m_mappings.front().second->set_parent( this );
    parent()->set_key_at( parent_index, m_mappings.front().first );
    // assert( is_sorted() );
}

#include <iostream>

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
    while( locator != end && key >= locator->first )
    {
        ++locator;
    }
    // locator->first is now the least key k such that aKey < k.
    // One before is the greatest key k such that aKey >= k.
    --locator;
    return locator->second;
}

std::size_t InternalNode::node_index( Node *node ) const
{
    const auto pred = [ node ]( const MappingType& v ){ return( v.second == node ); };
    const auto ff = std::find_if( m_mappings.begin(), m_mappings.end(), pred );
    assert( ff != m_mappings.end() );

    const auto index = std::distance( m_mappings.begin(), ff );
    assert( index >= 0 );

    return static_cast< std::size_t >( index );
}

Node* InternalNode::neighbor( std::size_t index ) const
{
    return m_mappings[ index ].second;
}

//
//
//
bool InternalNode::is_sorted() const
{
    const auto pred = []( const MappingType& ma, const MappingType& mb ){ return ma.first < mb.first ; };

    return std::is_sorted( m_mappings.begin(), m_mappings.end(), pred );
}


