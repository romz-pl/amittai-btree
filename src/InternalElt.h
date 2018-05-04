#ifndef ROMZ_AMITTAI_BTREE_INTERNALELT_H
#define ROMZ_AMITTAI_BTREE_INTERNALELT_H

#include "KeyType.h"
#include "Node.hpp"

class InternalElt
{
public:
    InternalElt( const KeyType &key, Node* node );
    ~InternalElt() = default;

public:
    KeyType m_key;

    Node* m_node;
};


#endif
