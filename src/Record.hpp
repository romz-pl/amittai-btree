#ifndef ROMZ_AMITTAI_BTREE_RECORD_H
#define ROMZ_AMITTAI_BTREE_RECORD_H

#include "Definitions.hpp"

class Record
{
public:
    explicit Record( ValueType value );
    ~Record() = default;

    ValueType value() const;
    std::string to_string() const;

private:
    ValueType m_value;
};

#endif

