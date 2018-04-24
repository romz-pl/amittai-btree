#include "KeyType.h"


KeyType::KeyType( std::int64_t val )
    : m_val( val )
{

}

bool KeyType::operator< ( const KeyType& key ) const
{
    return ( m_val < key.m_val );
}

bool KeyType::operator==( const KeyType& key ) const
{
    return ( m_val == key.m_val );
}

