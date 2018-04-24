#ifndef ROMZ_AMITTAI_BTREE_KEYTYPE_H
#define ROMZ_AMITTAI_BTREE_KEYTYPE_H

#include <cstdint>
#include "totally_ordered.h"

class KeyType : public totally_ordered< KeyType >
{
public:
    KeyType( std::int64_t val );
    ~KeyType() = default;

    KeyType( const KeyType& ) = default;
    KeyType& operator=( const KeyType& ) = default;

    KeyType( KeyType&& ) = default;
    KeyType& operator=( KeyType&& ) = default;


    bool operator< ( const KeyType& key ) const;
    bool operator==( const KeyType& key ) const;

    std::int64_t to_int64() const { return m_val; }

private:
    std::int64_t m_val;
};


#endif
