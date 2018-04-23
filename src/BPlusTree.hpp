#ifndef ROMZ_AMITTAI_BTREE_BPLUSTREE_H
#define ROMZ_AMITTAI_BTREE_BPLUSTREE_H

#include <tuple>
#include <vector>
#include "Definitions.hpp"
#include "Printer.hpp"
#include "Record.hpp"

class InternalNode;
class LeafNode;
class Node;


/// Main class providing the API for the Interactive B+ Tree.
class BPlusTree
{
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
    
    /// Print this B+ tree to stdout using a simple command-line
    /// ASCII graphic scheme.
    /// @param[in] aVerbose Determins whether printing should include addresses.
    void print( bool verbose = false );
    
    /// Print the bottom rank of this B+ tree, consisting of its leaves.
    /// This shows all the keys in the B+ tree in sorted order.
    /// @param[in] aVerbose Determins whether printing should include addresses.
    void print_leaves( bool verbose = false );
    
    /// Print the value associated with a given key, along with the address
    /// at which the tree stores that value.
    /// @param[in] aVerbose Determines whether printing should include addresses.
    void print_value( KeyType key, bool verbose = false );
    
    /// Print the path from the root to the leaf bearing key aKey.
    /// @param[in] aVerbose Determines whether printing should include addresses.
    void print_path_to( KeyType key, bool verbose = false );

    /// Print key, value, and address for each item in the range
    /// from aStart to aEnd, including both.
    void print_range( KeyType start, KeyType end );

    /// Remove all elements from the B+ tree. You can then build
    /// it up again by inserting new elements into it.
    void destroy_tree();
    
    /// Read elements to be inserted into the B+ tree from a text file.
    /// Each new element should consist of a single integer on a line by itself.
    /// This B+ tree treats each such input as both a new value and the key
    /// under which to store it.
    void read_input_from_file( std::string file_name );

private:
    void start_new_tree( KeyType key, ValueType value );
    void insert_into_leaf( KeyType key, ValueType value );
    void insert_into_parent( Node* old_node, KeyType key, Node* new_node );
    void remove_from_leaf( KeyType key );

    template< typename T > T* split( T* node );
    template< typename N > void coalesce_or_redistribute( N* node );
    template< typename N > void coalesce( N* neighbor_node, N* node, InternalNode* parent, int index );
    template< typename N > void redistribute( N* neighbor_node, N* node, InternalNode* parent, int index );

    void adjust_root();
    LeafNode* find_leaf_node( KeyType key, bool printing = false, bool verbose = false );
    void print_value( KeyType key, bool print_path, bool verbose );
    std::vector< EntryType > range( KeyType start, KeyType end );

private:
    const int m_order;
    Node* m_root;
    Printer m_printer;
};

#endif

