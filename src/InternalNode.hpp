#ifndef ROMZ_AMITTAI_BTREE_INTERNALNODE_H
#define ROMZ_AMITTAI_BTREE_INTERNALNODE_H

#include <queue>
#include <vector>
#include "Definitions.hpp"
#include "Node.hpp"

class InternalNode : public Node
{
public:
    InternalNode( std::uint32_t order, InternalNode* parent );
    ~InternalNode();

    using MappingType = std::pair< KeyType, Node* >;

    bool is_leaf() const override;
    std::uint32_t size() const override;
    std::uint32_t min_size() const override;
    std::uint32_t max_size() const override;
    std::string to_string( bool verbose = false ) const override;

    KeyType key_at( int index ) const;
    void set_key_at( int index, KeyType key );
    Node* first_child() const;
    void populate_new_root( Node* old_node, KeyType new_key, Node* new_node );
    std::uint32_t insert_node_after( Node* old_node, KeyType new_key, Node* new_node );
    void remove( int index );
    Node* remove_and_return_only_child();
    KeyType replace_and_return_first_key();


    void move_half_to( InternalNode* recipient );
    void move_all_to( InternalNode* recipient, int index_in_parent );
    void move_first_to_end_of( InternalNode* recipient );
    void move_last_to_front_of( InternalNode* recipient, int parent_index );


    Node* lookup( KeyType key ) const;
    int node_index( Node* node ) const;
    Node* neighbor( int index ) const;
    void queue_up_children( std::queue< Node* >* queue );

private:
    void copy_half_from( std::vector< MappingType >& mappings );
    void copy_all_from( std::vector< MappingType >& mappings );
    void copy_last_from( MappingType pair );
    void copy_first_from( MappingType pair, int parent_index );

private:
    std::vector< MappingType > m_mappings;

    // Key used where only the entry's pointer has meaning.
    const KeyType DUMMY_KEY{-1};
};


#endif

