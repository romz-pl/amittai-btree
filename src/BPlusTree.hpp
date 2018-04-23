#ifndef ROMZ_AMITTAI_BTREE_BPLUSTREE_H
#define ROMZ_AMITTAI_BTREE_BPLUSTREE_H

#include <tuple>
#include <vector>
#include "Definitions.hpp"
#include "Record.hpp"

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
    explicit BPlusTree( int order = DEFAULT_ORDER );
    ~BPlusTree() = default;
    
    /// The type used in the API for inserting a new key-value pair
    /// into the tree.  The third item is the type of the Node into
    /// which the key will be inserted.
    using EntryType = std::tuple< KeyType, ValueType, LeafNode* >;
    
    /// Returns true if this B+ tree has no keys or values.
    bool is_empty() const;

    Record* search( KeyType key );
    
    /// Insert a key-value pair into this B+ tree.
    void insert( KeyType key, ValueType value );
    
    /// Remove a key and its value from this B+ tree.
    void remove( KeyType key );
    

    /// Remove all elements from the B+ tree. You can then build
    /// it up again by inserting new elements into it.
    void destroy_tree();
    


private:
    void start_new_tree( KeyType key, ValueType value );
    void insert_into_leaf( KeyType key, ValueType value );
    void insert_into_parent( Node* old_node, KeyType key, Node* new_node );
    void remove_from_leaf( KeyType key );

    LeafNode* split( LeafNode* node );
    InternalNode* split( InternalNode* node );

    void coalesce_or_redistribute( LeafNode* node );
    void coalesce_or_redistribute( InternalNode* node );

    void coalesce( LeafNode* neighbor_node, LeafNode* node, InternalNode* parent, int index );
    void coalesce( InternalNode* neighbor_node, InternalNode* node, InternalNode* parent, int index );

    template< typename N > void redistribute( N* neighbor_node, N* node, InternalNode* parent, int index );

    void adjust_root();


    LeafNode* find_leaf_node( KeyType key );

private:
    const int m_order;
    Node* m_root;
};

#endif

