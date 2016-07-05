#pragma once
#ifndef _MYLEVELDB_HASH_H
#define _MYLEVELDB_HASH_H
#include <stddef.h>
#include <stdint.h>

namespace MyLevelDB
{
	extern uint32_t Hash(const char*, size_t, uint32_t);
}

#endif // !_MYLEVELDB_HASH_H
