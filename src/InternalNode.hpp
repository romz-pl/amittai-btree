#ifndef ROMZ_AMITTAI_BTREE_INTERNALNODE_H
#define ROMZ_AMITTAI_BTREE_INTERNALNODE_H


#include <vector>
#include "Definitions.hpp"
#include "Node.hpp"
#include "KeyType.h"
#include "InternalElt.h"

class InternalNode : public Node
{
    friend class Io;
    friend class Printer;

public:
    InternalNode( std::size_t order, InternalNode* parent );
    ~InternalNode();

    bool is_leaf() const override;
    std::size_t size() const override;
    std::size_t min_size() const override;
    std::size_t max_size() const override;


    KeyType key_at( std::size_t index ) const;
    void set_key_at( std::size_t index, KeyType key );
    Node* first_child() const;
    void populate_new_root( Node* old_node, KeyType new_key, Node* new_node );
    std::size_t insert_node_after( Node* old_node, KeyType new_key, Node* new_node );
    void remove( std::size_t index );
    Node* remove_and_return_only_child();
    KeyType replace_and_return_first_key();


    void move_half_to( InternalNode* recipient );
    void move_all_to( InternalNode* recipient, std::size_t index_in_parent );
    void move_first_to_end_of( InternalNode* recipient );
    void move_last_to_front_of( InternalNode* recipient, std::size_t parent_index );


    Node* lookup( KeyType key ) const;
    std::size_t node_index( Node* node ) const;
    Node* neighbor( std::size_t index ) const;


private:
    void copy_half_from( const std::vector< InternalElt >& mapp );
    void copy_all_from( const std::vector< InternalElt > &mapp );
    void copy_last_from( const InternalElt& pair );
    void copy_first_from( const InternalElt& pair, std::size_t parent_index );

    bool is_sorted() const;

private:
    std::vector< InternalElt > m_mappings;

    // Key used where only the entry's pointer has meaning.
    const KeyType DUMMY_KEY{-1};
};


#endif

