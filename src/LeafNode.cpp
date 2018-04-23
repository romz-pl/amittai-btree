#include <sstream>
#include "Exceptions.hpp"
#include "InternalNode.hpp"
#include "LeafNode.hpp"

LeafNode::LeafNode( std::uint32_t order )
    : Node( order )
    , m_next{ nullptr }
{

}

LeafNode::LeafNode( std::uint32_t order, InternalNode* parent )
    : Node( order, parent )
    , m_next{ nullptr }
{

}

LeafNode::~LeafNode()
{
    for( auto mapping : m_mappings )
    {
        delete mapping.second;
    }
}

bool LeafNode::is_leaf() const
{
    return true;
}

std::uint32_t LeafNode::size() const
{
    return static_cast<int>(m_mappings.size());
}

std::uint32_t LeafNode::min_size() const
{
    return order() / 2;
}

std::uint32_t LeafNode::max_size() const
{
    return order() - 1;
}

std::string LeafNode::to_string(bool verbose) const
{
    std::ostringstream keyToTextConverter;
    if (verbose) {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<" << m_mappings.size() << "> ";
    }
    bool first = true;
    for (auto mapping : m_mappings) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << mapping.first;
    }
    if (verbose) {
        keyToTextConverter << "[" << std::hex << m_next << ">";
    }
    return keyToTextConverter.str();
}

LeafNode* LeafNode::next() const
{
    return m_next;
}

void LeafNode::set_next( LeafNode* next )
{
    m_next = next;
}


std::uint32_t LeafNode::create_and_insert_record( KeyType key, ValueType value )
{
    Record* existingRecord = lookup(key);
    if (!existingRecord) {
        Record* newRecord = new Record(value);
        insert(key, newRecord);
    }
    return static_cast< std::uint32_t >( m_mappings.size() );
}

void LeafNode::insert( KeyType key, Record* record )
{
    auto insertionPoint = m_mappings.begin();
    auto end = m_mappings.end();
    while (insertionPoint != end && insertionPoint->first < key) {
        ++insertionPoint;
    }
    m_mappings.insert(insertionPoint, MappingType(key, record));
}

Record* LeafNode::lookup( KeyType key ) const
{
    for (auto mapping : m_mappings) {
        if (mapping.first == key) {
            return mapping.second;
        }
    }
    return nullptr;
}

void LeafNode::copy_range_starting_from( KeyType key, std::vector< EntryType >& vector )
{
    bool found = false;
    for (auto mapping : m_mappings) {
        if (mapping.first == key) {
            found = true;
        }
        if (found) {
            vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
        }
    }
}

void LeafNode::copy_range_until( KeyType key, std::vector< EntryType >& vector )
{
    bool found = false;
    for (auto mapping : m_mappings) {
        if (!found) {
            vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
        }
        if (mapping.first == key) {
            found = true;
        }
    }
}

void LeafNode::copy_range( std::vector< EntryType >& vector )
{
    for (auto mapping : m_mappings) {
        vector.push_back(std::make_tuple(mapping.first, mapping.second->value(), this));
    }
}


std::uint32_t LeafNode::remove_and_delete_record (KeyType key )
{
    auto removalPoint = m_mappings.begin();
    auto end = m_mappings.end();
    while (removalPoint != end && removalPoint->first != key) {
        ++removalPoint;
    }
    if (removalPoint == end) {
        throw RecordNotFoundException(key);
    }
    auto record = *removalPoint;
    m_mappings.erase(removalPoint);
    delete record.second;
    return static_cast< std::uint32_t >( m_mappings.size() );
}

KeyType LeafNode::first_key() const
{
    return m_mappings[0].first;
}

void LeafNode::move_half_to( LeafNode *recipient )
{
    recipient->copy_half_from(m_mappings);
    size_t size = m_mappings.size();
    for (size_t i = min_size(); i < size; ++i) {
        m_mappings.pop_back();
    }
}

void LeafNode::copy_half_from( std::vector< std::pair< KeyType, Record* > > &aMappings )
{
    for (size_t i = min_size(); i < aMappings.size(); ++i) {
        m_mappings.push_back(aMappings[i]);
    }
}

void LeafNode::move_all_to( LeafNode *recipient, int )
{
    recipient->copy_all_from(m_mappings);
    m_mappings.clear();
    recipient->set_next(next());
}

void LeafNode::copy_all_from( std::vector< std::pair< KeyType, Record* > > &aMappings )
{
    for (auto mapping : aMappings) {
        m_mappings.push_back(mapping);
    }
}

void LeafNode::move_first_to_end_of( LeafNode* recipient )
{
    recipient->copy_last_from(m_mappings.front());
    m_mappings.erase(m_mappings.begin());
    parent()->set_key_at(1, m_mappings.front().first);
}

void LeafNode::copy_last_from( MappingType pair )
{
    m_mappings.push_back(pair);
}

void LeafNode::move_last_to_front_of( LeafNode *recipient, int parent_index )
{
    recipient->copy_first_from(m_mappings.back(), parent_index);
    m_mappings.pop_back();
}

void LeafNode::copy_first_from( MappingType aPair, int parent_index )
{
    m_mappings.insert(m_mappings.begin(), aPair);
    parent()->set_key_at(parent_index, m_mappings.front().first);
}
