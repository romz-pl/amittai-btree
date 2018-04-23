#include <iostream>
#include <sstream>
#include "Exceptions.hpp"
#include "InternalNode.hpp"

InternalNode::InternalNode( int order )
    : Node( order )
{

}

InternalNode::InternalNode( int order, InternalNode *parent )
    : Node( order, parent )
{

}

InternalNode::~InternalNode()
{
    for (auto mapping : m_mappings)
    {
        delete mapping.second;
    }
}

bool InternalNode::is_leaf() const
{
    return false;
}

int InternalNode::size() const
{
    return static_cast<int>(m_mappings.size());
}

int InternalNode::min_size() const
{
    return order()/2;
}

int InternalNode::max_size() const
{
    // Includes the first entry, which
    // has key DUMMY_KEY and a value that
    // points to the left of the first positive key k1
    // (i.e., a node whose keys are all < k1).
    return order();
}

KeyType InternalNode::key_at( int index ) const
{
    return m_mappings[index].first;
}

void InternalNode::set_key_at( int index, KeyType key )
{
    m_mappings[index].first = key;
}

Node* InternalNode::first_child() const
{
    return m_mappings.front().second;
}

void InternalNode::populate_new_root( Node *old_node, KeyType new_key, Node *new_node )
{
    m_mappings.push_back(std::make_pair(DUMMY_KEY, old_node));
    m_mappings.push_back(std::make_pair(new_key, new_node));
}

int InternalNode::insert_node_after( Node *old_node, KeyType new_key, Node *new_node )
{
    auto iter = m_mappings.begin();
    for (; iter != m_mappings.end() && iter->second != old_node; ++iter);
    m_mappings.insert(iter + 1, std::make_pair(new_key, new_node));
    return size();
}

void InternalNode::remove( int index )
{
    m_mappings.erase(m_mappings.begin() + index);
}

Node* InternalNode::remove_and_return_only_child()
{
    Node* firstChild = m_mappings.front().second;
    m_mappings.pop_back();
    return firstChild;
}

KeyType InternalNode::replace_and_return_first_key()
{
    KeyType newKey = m_mappings[0].first;
    m_mappings[0].first = DUMMY_KEY;
    return newKey;
}

void InternalNode::move_half_to( InternalNode *recipient )
{
    recipient->copy_half_from(m_mappings);
    size_t size = m_mappings.size();
    for (size_t i = min_size(); i < size; ++i) {
        m_mappings.pop_back();
    }
}

void InternalNode::copy_half_from( std::vector< MappingType > &mappings )
{
    for (size_t i = min_size(); i < mappings.size(); ++i) {
        mappings[i].second->set_parent(this);
        m_mappings.push_back(mappings[i]);
    }
}

void InternalNode::move_all_to( InternalNode *recipient, int index_in_parent )
{
    m_mappings[0].first = parent()->key_at(index_in_parent);
    recipient->copy_all_from(m_mappings);
    m_mappings.clear();
}

void InternalNode::copy_all_from( std::vector< MappingType > &mappings )
{
    for (auto mapping : mappings) {
        mapping.second->set_parent(this);
        m_mappings.push_back(mapping);
    }
}

void InternalNode::move_first_to_end_of( InternalNode *recipient )
{
    recipient->copy_last_from(m_mappings.front());
    m_mappings.erase(m_mappings.begin());
    parent()->set_key_at(1, m_mappings.front().first);
}

void InternalNode::copy_last_from( MappingType pair )
{
    m_mappings.push_back(pair);
    m_mappings.back().second->set_parent(this);
}

void InternalNode::move_last_to_front_of( InternalNode *recipient, int parent_index )
{
    recipient->copy_first_from(m_mappings.back(), parent_index);
    m_mappings.pop_back();
}

void InternalNode::copy_first_from( MappingType pair, int parent_index )
{
    m_mappings.front().first = parent()->key_at(parent_index);
    m_mappings.insert(m_mappings.begin(), pair);
    m_mappings.front().first = DUMMY_KEY;
    m_mappings.front().second->set_parent(this);
    parent()->set_key_at(parent_index, m_mappings.front().first);
}

Node* InternalNode::lookup( KeyType key ) const
{
    auto locator = m_mappings.begin();
    auto end = m_mappings.end();
    while (locator != end && key >= locator->first) {
        ++locator;
    }
    // locator->first is now the least key k such that aKey < k.
    // One before is the greatest key k such that aKey >= k.
    --locator;
    return locator->second;
}

int InternalNode::node_index( Node *node ) const
{
    for (int i = 0; i < size(); ++i) {
        if (m_mappings[i].second == node) {
            return static_cast<int>(i);
        }
    }
    throw NodeNotFoundException(node->to_string(), to_string());
}

Node* InternalNode::neighbor( int index ) const
{
    return m_mappings[index].second;
}

std::string InternalNode::to_string( bool verbose ) const
{
    if (m_mappings.empty()) {
        return "";
    }
    std::ostringstream keyToTextConverter;
    if (verbose) {
        keyToTextConverter << "[" << std::hex << this << std::dec << "]<" << m_mappings.size() << "> ";
    }
    auto entry = verbose ? m_mappings.begin() : m_mappings.begin() + 1;
    auto end = m_mappings.end();
    bool first = true;
    while (entry != end) {
        if (first) {
            first = false;
        } else {
            keyToTextConverter << " ";
        }
        keyToTextConverter << std::dec << entry->first;
        if (verbose) {
            keyToTextConverter << "(" << std::hex << entry->second << std::dec << ")";
        }
        ++entry;
    }
    return keyToTextConverter.str();
}

void InternalNode::queue_up_children( std::queue< Node* >* queue )
{
    for (auto mapping : m_mappings) {
        queue->push(mapping.second);
    }
}
