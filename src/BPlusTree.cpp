#include <iostream>
#include <string>
#include "BPlusTree.hpp"
#include "Exceptions.hpp"
#include "InternalNode.hpp"
#include "LeafNode.hpp"
#include "Node.hpp"

BPlusTree::BPlusTree( int order )
    : m_order{ order }
    , m_root{ nullptr }
{

}

void BPlusTree::destroy_tree()
{
    if (m_root->is_leaf()) {
        delete static_cast<LeafNode*>(m_root);
    } else {
        delete static_cast<InternalNode*>(m_root);
    }
    m_root = nullptr;
}

bool BPlusTree::is_empty() const
{
    return !m_root;
}


Record* BPlusTree::search( KeyType key )
{
    LeafNode* leafNode = find_leaf_node( key );
    if( !leafNode )
    {
        return nullptr;
    }
    return leafNode->lookup( key );
}



// INSERTION

void BPlusTree::insert( KeyType key, ValueType value )
{
    if (is_empty()) {
        start_new_tree(key, value);
    } else {
        insert_into_leaf(key, value);
    }
}

void BPlusTree::start_new_tree( KeyType key, ValueType value )
{
    LeafNode* newLeafNode = new LeafNode(m_order);
    newLeafNode->create_and_insert_record(key, value);
    m_root = newLeafNode;
}

void BPlusTree::insert_into_leaf( KeyType key, ValueType value )
{
    LeafNode* leafNode = find_leaf_node(key);
    if (!leafNode) {
        throw LeafNotFoundException(key);
    }
    int newSize = leafNode->create_and_insert_record(key, value);
    if (newSize > leafNode->max_size()) {
        LeafNode* newLeaf = split(leafNode);
        newLeaf->set_next(leafNode->next());
        leafNode->set_next(newLeaf);
        KeyType newKey = newLeaf->first_key();
        insert_into_parent(leafNode, newKey, newLeaf);
    }
}

void BPlusTree::insert_into_parent(  Node *old_node, KeyType key, Node *new_node )
{
    InternalNode* parent = static_cast<InternalNode*>(old_node->parent());
    if (parent == nullptr) {
        m_root = new InternalNode(m_order);
        parent = static_cast<InternalNode*>(m_root);
        old_node->set_parent(parent);
        new_node->set_parent(parent);
        parent->populate_new_root(old_node, key, new_node);
    } else {
        int newSize = parent->insert_node_after(old_node, key, new_node);
        if (newSize > parent->max_size()) {
            InternalNode* newNode = split(parent);
            KeyType newKey = newNode->replace_and_return_first_key();
            insert_into_parent(parent, newKey, newNode);
        }
    }
}

template< typename T >
T* BPlusTree::split( T* node )
{
    T* newNode = new T(m_order, node->parent());
    node->move_half_to(newNode);
    return newNode;
}

// REMOVAL


void BPlusTree::remove( KeyType key )
{
    if (is_empty()) {
        return;
    } else {
        remove_from_leaf(key);
    }
}

void BPlusTree::remove_from_leaf( KeyType key )
{
    LeafNode* leafNode = find_leaf_node(key);
    if (!leafNode) {
        return;
    }
    if (!leafNode->lookup(key)) {
        return;
    }
    int newSize = leafNode->remove_and_delete_record(key);
    if (newSize < leafNode->min_size()) {
        coalesce_or_redistribute(leafNode);
    }
}

template< typename N >
void BPlusTree::coalesce_or_redistribute( N* node )
{
    if (node->is_root()) {
        adjust_root();
        return;
    }
    auto parent = static_cast<InternalNode*>(node->parent());
    int indexOfNodeInParent = parent->node_index(node);
    int neighborIndex = (indexOfNodeInParent == 0) ? 1 : indexOfNodeInParent - 1;
    N* neighborNode = static_cast<N*>(parent->neighbor(neighborIndex));
    if (node->size() + neighborNode->size() <= neighborNode->max_size()) {
        coalesce(neighborNode, node, parent, indexOfNodeInParent);
    } else {
        redistribute(neighborNode, node, parent, indexOfNodeInParent);
    }
}

template< typename N >
void BPlusTree::coalesce( N* neighbor_node, N* node, InternalNode* parent, int index )
{
    if (index == 0) {
        std::swap(node, neighbor_node);
        index = 1;
    }
    node->move_all_to(neighbor_node, index);
    parent->remove(index);
    if (parent->size() < parent->min_size()) {
        coalesce_or_redistribute(parent);
    }
    delete node;
}

template< typename N >
void BPlusTree::redistribute( N* neighbor_node, N* node, InternalNode* /*aParent*/, int index )
{
    if (index == 0) {
        neighbor_node->move_first_to_end_of(node);
    } else {
        neighbor_node->move_last_to_front_of(node, index);
    }
}

void BPlusTree::adjust_root()
{
    if (!m_root->is_leaf() && m_root->size() == 1) {
        auto discardedNode = static_cast<InternalNode*>(m_root);
        m_root = static_cast<InternalNode*>(m_root)->remove_and_return_only_child();
        m_root->set_parent(nullptr);
        delete discardedNode;
    } else if (!m_root->size()){
        delete m_root;
        m_root = nullptr;
    }
}

LeafNode* BPlusTree::find_leaf_node( KeyType key, bool printing, bool verbose )
{
    if (is_empty()) {
        if (printing) {
            std::cout << "Not found: empty tree." << std::endl;
        }
        return nullptr;
    }
    auto node = m_root;
    if (printing) {
        std::cout << "Root: ";
        if (m_root->is_leaf()) {
            std::cout << "\t" << static_cast<LeafNode*>(m_root)->to_string(verbose);
        } else {
            std::cout << "\t" << static_cast<InternalNode*>(m_root)->to_string(verbose);
        }
        std::cout << std::endl;
    }
    while (!node->is_leaf()) {
        auto internalNode = static_cast<InternalNode*>(node);
        if (printing && node != m_root) {
            std::cout << "\tNode: " << internalNode->to_string(verbose) << std::endl;
        }
        node = internalNode->lookup(key);
    }
    return static_cast<LeafNode*>(node);
}

