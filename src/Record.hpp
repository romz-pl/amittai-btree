#ifndef ROMZ_AMITTAI_BTREE_RECORD_H
#define ROMZ_AMITTAI_BTREE_RECORD_H

#include "Definitions.hpp"

class Record
{
public:
    explicit Record(ValueType aValue);
    ValueType value() const;
    std::string toString() const;
private:
    Record() : fValue(0) {}
    ValueType fValue;
};

#endif

