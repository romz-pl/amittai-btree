#include <stdexcept>
#include <algorithm>
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
    Record* existing_record = lookup( key );
    if( !existing_record )
    {
        Record* new_record = new Record( value );
        insert( key, new_record );
    }
    return m_mappings.size();
}

void LeafNode::insert( KeyType key, Record* record )
{
    auto insertion_point = m_mappings.begin();
    auto end = m_mappings.end();
    while( insertion_point != end && insertion_point->first < key )
    {
        ++insertion_point;
    }
    m_mappings.insert( insertion_point, MappingType( key, record ) );
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
    auto removal_point = m_mappings.begin();
    auto end = m_mappings.end();
    while( removal_point != end && removal_point->first != key )
    {
        ++removal_point;
    }

    if( removal_point == end )
    {
        throw std::runtime_error( "Record Not Found" );
    }

    auto record = *removal_point;
    m_mappings.erase( removal_point );
    delete record.second;
    return m_mappings.size();
}

KeyType LeafNode::first_key() const
{
    return m_mappings[ 0 ].first;
}

void LeafNode::move_half_to( LeafNode *recipient )
{
    recipient->copy_half_from( m_mappings );
    const std::size_t size = m_mappings.size();
    for( std::size_t i = min_size(); i < size; ++i )
    {
        m_mappings.pop_back();
    }
}

void LeafNode::copy_half_from( std::vector< std::pair< KeyType, Record* > > &mappings )
{
    for( std::size_t i = min_size(); i < mappings.size(); ++i )
    {
        m_mappings.push_back( mappings[ i ] );
    }
}

void LeafNode::move_all_to( LeafNode *recipient )
{
    recipient->copy_all_from( m_mappings );
    m_mappings.clear();
    recipient->set_next( next() );
}

void LeafNode::copy_all_from( std::vector< std::pair< KeyType, Record* > > &mappings )
{
    for( auto m : mappings )
    {
        m_mappings.push_back( m );
    }
}

void LeafNode::move_first_to_end_of( LeafNode* recipient )
{
    recipient->copy_last_from( m_mappings.front() );
    m_mappings.erase( m_mappings.begin() );
    parent()->set_key_at( 1, m_mappings.front().first );
}

void LeafNode::copy_last_from( MappingType pair )
{
    m_mappings.push_back( pair );
}

void LeafNode::move_last_to_front_of( LeafNode *recipient, std::size_t parent_index )
{
    recipient->copy_first_from( m_mappings.back(), parent_index );
    m_mappings.pop_back();
}

void LeafNode::copy_first_from( MappingType aPair, std::size_t parent_index )
{
    m_mappings.insert( m_mappings.begin(), aPair );
    parent()->set_key_at( parent_index, m_mappings.front().first );
}
