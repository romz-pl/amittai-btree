#ifndef ROMZ_AMITTAI_BTREE_IO_H
#define ROMZ_AMITTAI_BTREE_IO_H

#include <string>
#include "Printer.hpp"
#include "BPlusTree.hpp"
#include "LeafNode.hpp"

class Io
{
public:
    Io( BPlusTree& tree );
    ~Io() = default;

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

    /// Read elements to be inserted into the B+ tree from a text file.
    /// Each new element should consist of a single integer on a line by itself.
    /// This B+ tree treats each such input as both a new value and the key
    /// under which to store it.
    void read_input_from_file( std::string file_name );



private:
    std::vector< BPlusTree::EntryType > range( KeyType start, KeyType end );
    LeafNode* find_leaf_node( KeyType key, bool printing = false, bool verbose = false );
    void print_value( KeyType key, bool print_path, bool verbose );


    void leaf_node_copy_range_starting_from( LeafNode* leaf, KeyType key, std::vector< LeafNode::EntryType >& vector );
    void leaf_node_copy_range_until( LeafNode* leaf, KeyType key, std::vector< LeafNode::EntryType >& vector );
    void leaf_node_copy_range( LeafNode* leaf, std::vector< LeafNode::EntryType >& vector );




private:
    Printer m_printer;
    BPlusTree& m_tree;
};


#endif

