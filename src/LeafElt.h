#ifndef ROMZ_AMITTAI_BTREE_LEAFELT_H
#define ROMZ_AMITTAI_BTREE_LEAFELT_H

#include "KeyType.h"
#include "Record.hpp"

class LeafElt
{
public:
    LeafElt( KeyType key, Record* record );
    ~LeafElt() = default;

public:
    KeyType m_key;

    Record* m_record;
};


#endif
