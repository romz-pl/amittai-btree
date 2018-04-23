#ifndef ROMZ_AMITTAI_BTREE_LEAFNODE_H
#define ROMZ_AMITTAI_BTREE_LEAFNODE_H

#include <tuple>
#include <utility>
#include <vector>
#include "Node.hpp"
#include "Record.hpp"

class LeafNode : public Node
{
public:
    explicit LeafNode( std::uint32_t order );
    LeafNode( std::uint32_t order, InternalNode* parent );
    ~LeafNode();

    using MappingType = std::pair< KeyType, Record* >;
    using EntryType = std::tuple< KeyType, ValueType, LeafNode* >;

    bool is_leaf() const override;
    std::uint32_t size() const override;
    std::uint32_t min_size() const override;
    std::uint32_t max_size() const override;
    std::string to_string( bool verbose = false ) const override;

    LeafNode* next() const;
    void set_next( LeafNode* next );
    std::uint32_t create_and_insert_record( KeyType key, ValueType value );
    void insert( KeyType key, Record* record );
    Record* lookup( KeyType key ) const;
    std::uint32_t remove_and_delete_record( KeyType key );
    KeyType first_key() const;


    void move_half_to( LeafNode* recipient );
    void move_all_to( LeafNode* recipient, int /* Unused */ );
    void move_first_to_end_of( LeafNode* recipient );
    void move_last_to_front_of( LeafNode* recipient, int parent_index );


    void copy_range_starting_from( KeyType key, std::vector< EntryType >& vector );
    void copy_range_until( KeyType key, std::vector< EntryType >& vector );
    void copy_range( std::vector<EntryType>& vector );

private:
    void copy_half_from( std::vector< MappingType >& mappings );
    void copy_all_from( std::vector< MappingType >& mappings );
    void copy_last_from( MappingType pair );
    void copy_first_from( MappingType pair, int parent_index );

private:
    std::vector< MappingType > m_mappings;

    LeafNode* m_next;
};

#endif
