#pragma once
#ifndef _MYLEVELDB_ARENA_H
#define _MYLEVELDB_ARENA_H

#include <cstddef>
#include <vector>
#include <assert.h>
#include <stdint.h>

namespace MyLevelDB
{
	class Arena
	{
	public:
		Arena();
		~Arena();

		char* Allocate(size_t bytes);
		char* AllocateAligned(size_t);

		size_t MemoryUsage() const
		{
			return _block_memory + _blocks.capacity() * sizeof(char*);
		}
	private:
		char* AllocateFallback(size_t bytes);
		char* AllocateNewBlock(size_t block_bytes);

		char* _alloc_ptr;
		size_t _alloc_bytes_remaining;

		std::vector<char*> _blocks;
		size_t _block_memory;

		//½ûÖ¹¸³ÖµÓë¿½±´
		Arena(const Arena&) = delete;
		void operator=(const Arena&) = delete;
	};

	inline char* Arena::Allocate(size_t bytes)
	{
		assert(bytes > 0);
		if (bytes <= _alloc_bytes_remaining)
		{
			char* result = _alloc_ptr;
			_alloc_ptr += bytes;
			_alloc_bytes_remaining -= bytes;
			return result;
		}

		return AllocateFallback(bytes);
	}
}

#endif // !_MYLEVELDB_ARENA_H