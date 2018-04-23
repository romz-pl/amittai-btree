#ifndef ROMZ_AMITTAI_BTREE_DEFINITIONS_H
#define ROMZ_AMITTAI_BTREE_DEFINITIONS_H


#include <cstdlib>

// #define VERSION "2.0.2"

const std::size_t DEFAULT_ORDER{4};

// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
const std::size_t MIN_ORDER{DEFAULT_ORDER - 1};
const std::size_t MAX_ORDER{20};

using KeyType = int64_t;
using ValueType = int64_t;

#endif


