#ifndef ROMZ_AMITTAI_BTREE_PRINTER_H
#define ROMZ_AMITTAI_BTREE_PRINTER_H

#include <queue>

class Node;

class Printer
{
public:
    Printer();
    bool verbose() const;
    void set_verbose( bool verbose );
    void print_tree( Node* root ) const;
    void print_leaves( Node* root );

private:
    void print_empty_tree() const;
    void print_non_empty_tree( Node* root ) const;
    void print_current_rank( std::queue< Node* >* current_rank, std::queue< Node* >* next_rank ) const;

private:
    bool m_verbose;
};

#endif
