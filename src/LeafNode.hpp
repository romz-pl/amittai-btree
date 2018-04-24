#ifndef ROMZ_AMITTAI_BTREE_LEAFNODE_H
#define ROMZ_AMITTAI_BTREE_LEAFNODE_H

#include <tuple>
#include <utility>
#include <vector>
#include "Node.hpp"
#include "Record.hpp"
#include "KeyType.h"

class LeafNode : public Node
{
    friend class Io;
    friend class Printer;
public:
    LeafNode( std::size_t order, InternalNode* parent );
    ~LeafNode();

    using MappingType = std::pair< KeyType, Record* >;
    using EntryType = std::tuple< KeyType, ValueType, LeafNode* >;

    bool is_leaf() const override;
    std::size_t size() const override;
    std::size_t min_size() const override;
    std::size_t max_size() const override;

    LeafNode* next() const;
    void set_next( LeafNode* next );
    std::size_t create_and_insert_record( KeyType key, ValueType value );
    void insert( KeyType key, Record* record );
    Record* lookup( KeyType key ) const;
    std::size_t remove_and_delete_record( KeyType key );
    KeyType first_key() const;


    void move_half_to( LeafNode* recipient );
    void move_all_to( LeafNode* recipient );
    void move_first_to_end_of( LeafNode* recipient );
    void move_last_to_front_of( LeafNode* recipient, std::size_t parent_index );




private:
    void copy_half_from( std::vector< MappingType >& mappings );
    void copy_all_from( std::vector< MappingType >& mappings );
    void copy_last_from( MappingType pair );
    void copy_first_from( MappingType pair, std::size_t parent_index );

private:
    std::vector< MappingType > m_mappings;

    LeafNode* m_next;
};

#endif
