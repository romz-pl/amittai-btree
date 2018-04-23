#ifndef ROMZ_AMITTAI_BTREE_PRINTER_H
#define ROMZ_AMITTAI_BTREE_PRINTER_H

#include <queue>

class Node;
class InternalNode;
class LeafNode;

class Printer
{
public:
    Printer();
    bool verbose() const;
    void set_verbose( bool verbose );
    void print_tree( Node* root ) const;
    void print_leaves( Node* root );

    static std::string to_string( const Node* node, bool verbose = false );

    static std::string to_string( const LeafNode* leaf, bool verbose = false );
    static std::string to_string( const InternalNode* internal, bool verbose = false );

private:
    void print_empty_tree() const;
    void print_non_empty_tree( Node* root ) const;
    void print_current_rank( std::queue< Node* >* current_rank, std::queue< Node* >* next_rank ) const;

    void internal_node_queue_up_children( InternalNode* internal, std::queue< Node* >* queue ) const;

private:
    bool m_verbose;
};

#endif
