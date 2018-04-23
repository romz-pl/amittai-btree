#ifndef ROMZ_AMITTAI_BTREE_NODE_H
#define ROMZ_AMITTAI_BTREE_NODE_H


#include <cstdint>
#include "Definitions.hpp"



class InternalNode;


class Node
{
public:
    Node( std::uint32_t order, InternalNode *parent );
    virtual ~Node() = default;

    std::uint32_t order() const;
    InternalNode* parent() const;
    void set_parent( InternalNode* parent );
    bool is_root() const;

    virtual bool is_leaf() const = 0;
    virtual std::uint32_t size() const = 0;
    virtual std::uint32_t min_size() const = 0;
    virtual std::uint32_t max_size() const = 0;

private:
    const std::uint32_t m_order;

    InternalNode* m_parent;
};

#endif
