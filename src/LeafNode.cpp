#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "LeafNode.hpp"
#include "InternalNode.hpp"

LeafNode::LeafNode( std::size_t order, InternalNode* parent )
    : Node( order, parent )
    , m_next{ nullptr }
{

}

LeafNode::~LeafNode()
{
    for( auto m : m_mappings )
    {
        delete m.second;
    }
}

bool LeafNode::is_leaf() const
{
    return true;
}

std::size_t LeafNode::size() const
{
    return m_mappings.size();
}

std::size_t LeafNode::min_size() const
{
    return order() / 2;
}

std::size_t LeafNode::max_size() const
{
    return order() - 1;
}


LeafNode* LeafNode::next() const
{
    return m_next;
}

void LeafNode::set_next( LeafNode* next )
{
    m_next = next;
}


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
    return m_mappings.size();
}

void LeafNode::insert( KeyType key, Record* record )
{
    assert( is_sorted() );

    const auto pred = [ key ]( const MappingType& m ){ return m.first >= key; };
    const auto insertion_point = std::find_if( m_mappings.begin(), m_mappings.end(), pred );
    m_mappings.insert( insertion_point, MappingType( key, record ) );

    assert( is_sorted() );
}

Record* LeafNode::lookup( KeyType key ) const
{
    const auto pred = [ key ]( const MappingType& m ){ return m.first == key; };
    const auto it = std::find_if( m_mappings.begin(), m_mappings.end(), pred );

    if( it != m_mappings.end() )
    {
        return it->second;
    }

    return nullptr;
}



std::size_t LeafNode::remove_and_delete_record( KeyType key )
{
    assert( is_sorted() );

    const auto pred = [ key ]( const MappingType& m ){ return m.first == key; };
    const auto removal_point = std::find_if( m_mappings.begin(), m_mappings.end(), pred );

    if( removal_point == m_mappings.end() )
    {
        throw std::runtime_error( "Key Not Found" );
    }

    delete removal_point->second;
    m_mappings.erase( removal_point );

    assert( is_sorted() );

    return m_mappings.size();
}

KeyType LeafNode::first_key() const
{
    return m_mappings[ 0 ].first;
}

void LeafNode::move_half_to( LeafNode *recipient )
{
    assert( is_sorted() );

    recipient->copy_half_from( m_mappings );
    const auto beg = m_mappings.begin() + min_size();
    m_mappings.erase( beg, m_mappings.end() );

    assert( is_sorted() );
}

void LeafNode::copy_half_from( const std::vector< std::pair< KeyType, Record* > > &mapp )
{
    assert( is_sorted() );

    m_mappings.reserve( m_mappings.size() + mapp.size() - min_size() );
    const auto beg = mapp.begin() + min_size();
    m_mappings.insert( m_mappings.end(), beg, mapp.end() );

    assert( is_sorted() );
}

void LeafNode::move_all_to( LeafNode *recipient )
{
    assert( is_sorted() );

    recipient->copy_all_from( m_mappings );
    m_mappings.clear();
    recipient->set_next( next() );

    assert( is_sorted() );
}

void LeafNode::copy_all_from( const std::vector< std::pair< KeyType, Record* > > &mapp )
{
    assert( is_sorted() );
    m_mappings.reserve( m_mappings.size() + mapp.size() );
    m_mappings.insert( m_mappings.end(), mapp.begin(), mapp.end() );
    assert( is_sorted() );
}

void LeafNode::move_first_to_end_of( LeafNode* recipient )
{
    assert( is_sorted() );

    recipient->copy_last_from( m_mappings.front() );
    m_mappings.erase( m_mappings.begin() );
    parent()->set_key_at( 1, m_mappings.front().first );

    assert( is_sorted() );
}

void LeafNode::copy_last_from( const MappingType& pair )
{
    assert( is_sorted() );
    m_mappings.push_back( pair );
    assert( is_sorted() );
}

void LeafNode::move_last_to_front_of( LeafNode *recipient, std::size_t parent_index )
{
    assert( is_sorted() );
    recipient->copy_first_from( m_mappings.back(), parent_index );
    m_mappings.pop_back();
    assert( is_sorted() );
}

void LeafNode::copy_first_from( const MappingType& pair, std::size_t parent_index )
{
    assert( is_sorted() );
    m_mappings.insert( m_mappings.begin(), pair );
    parent()->set_key_at( parent_index, m_mappings.front().first );
    assert( is_sorted() );
}

//
//
//
bool LeafNode::is_sorted() const
{
    const auto pred = []( const MappingType& ma, const MappingType& mb ){ return ma.first < mb.first ; };

    return std::is_sorted( m_mappings.begin(), m_mappings.end(), pred );
}
