#ifndef ROMZ_AMITTAI_BTREE_INTERNALNODE_H
#define ROMZ_AMITTAI_BTREE_INTERNALNODE_H

//
// 1. Let us consider a node containing m pointers (m <= n).
//    Available indexes: i = 1, 2, ..., m.
//
// 2. Valid pointers: P_1, P_2, ..., P_m
//
// 3. Valid keys: K_1, K_2, ..., K_{m-1}
//
// 4. For i = 2, 3, ..., (m − 1) pointer P_i points to the subtree that contains
//    search-key values:
//       a) greater than or equal to K_{i−1} and
//       b) less than K_i and.
//
// 5. Pointer P_m points to the part of the subtree that contains those key values
//    greater than or equal to K_{m−1}.
//
// 6. Pointer P_1 points to the part of the subtree that contains those
//    search-key values less than K_1.
//


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
    InternalNode( BPlusTree *tree, InternalNode* get_parent );
    ~InternalNode();

    bool is_leaf() const override;
    std::size_t size() const override;


    KeyType key_at( std::size_t index ) const;
    void set_key_at( std::size_t index, KeyType key );

    Node* first_child() const;

    void populate_new_root( Node* old_node, KeyType new_key, Node* new_node );
    void insert_node_after( Node* old_node, KeyType new_key, Node* new_node );

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

    InternalNode* internal() override;
    const InternalNode* internal() const override;


private:
    void copy_half_from( const std::vector< InternalElt >& ve );
    void copy_all_from( const std::vector< InternalElt >& ve );
    void copy_last_from( const InternalElt& pair );
    void copy_first_from( const InternalElt& pair, std::size_t parent_index );

    bool is_sorted() const;

private:
    std::vector< InternalElt > m_elt;

    // Key used where only the entry's pointer has meaning.
    const KeyType DUMMY_KEY{-1};
};


#endif

