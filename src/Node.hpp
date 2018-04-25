#ifndef ROMZ_AMITTAI_BTREE_NODE_H
#define ROMZ_AMITTAI_BTREE_NODE_H

#include "Definitions.hpp"
#include "BPlusTree.hpp"

class InternalNode;
class LeafNode;

class Node
{
public:
    Node( BPlusTree *tree, InternalNode *parent );
    virtual ~Node() = default;

    InternalNode* parent() const;
    void set_parent( InternalNode* parent );
    bool is_root() const;

    virtual InternalNode* internal();
    virtual const InternalNode* internal() const;

    virtual LeafNode* leaf();
    virtual const LeafNode* leaf() const;


    virtual bool is_leaf() const = 0;
    virtual std::size_t size() const = 0;

protected:
    BPlusTree *m_tree;

private:
    InternalNode* m_parent;


};

#endif
