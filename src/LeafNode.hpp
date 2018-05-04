#ifndef ROMZ_AMITTAI_BTREE_LEAFNODE_H
#define ROMZ_AMITTAI_BTREE_LEAFNODE_H


#include <vector>
#include "Node.hpp"
#include "Record.hpp"
#include "KeyType.h"
#include "LeafElt.h"

class LeafNode : public Node
{
    friend class Io;
    friend class Printer;

public:
    LeafNode( BPlusTree *tree, InternalNode* parent );
    ~LeafNode();

    bool is_leaf() const override;
    std::size_t size() const override;

    LeafNode* next() const;
    void set_next( LeafNode* next );
    void create_and_insert_record( const KeyType& key, ValueType value );
    void insert( const KeyType& key, Record* record );
    Record* lookup( const KeyType& key ) const;
    std::size_t remove_and_delete_record( const KeyType& key );
    KeyType first_key() const;

    void move_half_to( LeafNode* recipient );
    void move_all_to( LeafNode* recipient );
    void move_first_to_end_of( LeafNode* recipient );
    void move_last_to_front_of( LeafNode* recipient, std::size_t parent_index );

    LeafNode* leaf() override;
    const LeafNode* leaf() const override;

private:
    void copy_half_from( const std::vector< LeafElt >& ve );
    void copy_all_from( const std::vector< LeafElt >& ve );
    void copy_last_from( const LeafElt &pair );
    void copy_first_from( const LeafElt &pair, std::size_t parent_index );

    bool is_sorted() const;

private:
    std::vector< LeafElt > m_elt;

    LeafNode* m_next;
};

#endif
