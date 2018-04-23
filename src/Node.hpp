#ifndef ROMZ_AMITTAI_BTREE_NODE_H
#define ROMZ_AMITTAI_BTREE_NODE_H

#include <string>
#include "Definitions.hpp"

// Key used where only the entry's pointer has meaning.
const KeyType DUMMY_KEY{-1};

class InternalNode;

// Abstract class.
class Node
{
public:
    explicit Node( int order );
    Node( int order, InternalNode *parent );
    virtual ~Node() = default;

    int order() const;
    InternalNode* parent() const;
    void set_parent( InternalNode* parent);
    bool is_root() const;

    virtual bool is_leaf() const = 0;
    virtual int size() const = 0;
    virtual int min_size() const = 0;
    virtual int max_size() const = 0;
    virtual std::string to_string(bool aVerbose = false) const = 0;

private:
    const int m_order;

    InternalNode* m_parent;
};

#endif
