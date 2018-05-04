#ifndef ROMZ_AMITTAI_BTREE_BPLUSTREE_H
#define ROMZ_AMITTAI_BTREE_BPLUSTREE_H

#include "Definitions.hpp"
#include "Record.hpp"
#include "KeyType.h"

class InternalNode;
class LeafNode;
class Node;


/// Main class providing the API for the Interactive B+ Tree.
class BPlusTree
{
    friend class Io;

public:
    /// Sole constructor.  Accepts an optional order for the B+ Tree.
    /// The default order will provide a reasonable demonstration of the
    /// data structure and its operations.
    explicit BPlusTree( std::size_t order );
    ~BPlusTree();
    
    /// Returns true if this B+ tree has no keys or values.
    bool is_empty() const;

    Record* search( const KeyType& key );
    
    /// Insert a key-value pair into this B+ tree.
    void insert( const KeyType& key, ValueType value );
    
    /// Remove a key and its value from this B+ tree.
    void remove( const KeyType& key );
    

    /// Remove all elements from the B+ tree. You can then build
    /// it up again by inserting new elements into it.
    void destroy_tree();

    std::size_t leaf_min_size() const;
    std::size_t leaf_max_size() const;

    std::size_t internal_min_size() const;
    std::size_t internal_max_size() const;
    


private:
    void start_new_tree( const KeyType& key, ValueType value );
    void insert_into_leaf( const KeyType& key, ValueType value );
    void insert_into_parent( Node* old_node, const KeyType& key, Node* new_node );
    void remove_from_leaf( const KeyType& key );

    LeafNode* split( LeafNode* node );
    InternalNode* split( InternalNode* node );

    void coalesce_or_redistribute( LeafNode* node );
    void coalesce_or_redistribute( InternalNode* node );

    void coalesce( LeafNode* neighbor_node, LeafNode* node, InternalNode* parent, std::size_t index );
    void coalesce( InternalNode* neighbor_node, InternalNode* node, InternalNode* parent, std::size_t index );

    void redistribute( LeafNode* neighbor_node, LeafNode* node, std::size_t index );
    void redistribute( InternalNode* neighbor_node, InternalNode* node, std::size_t index );

    void adjust_root();


    LeafNode* find_leaf_node( const KeyType& key );

private:
    const std::size_t m_order;
    Node* m_root;
};

#endif

