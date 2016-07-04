#include "../MyLevelDB/arena.h"
#include <assert.h>

namespace MyLevelDB
{
	static const int kBlockSize = 4096;
	Arena::Arena()
	{
		_block_memory = 0;
		_alloc_ptr = NULL;
		_alloc_bytes_remaining = 0;
	}
	Arena::~Arena()
	{
		for (size_t i = 0; i < _blocks.size(); ++i)
			delete[] _blocks[i];
	}
	char* Arena::AllocateFallback(size_t bytes)
	{
		if (bytes > kBlockSize / 4)
		{
			char* result = AllocateNewBlock(bytes);
			return result;
		}
		_alloc_ptr = AllocateNewBlock(kBlockSize);
		_alloc_bytes_remaining = kBlockSize;

		char* result = _alloc_ptr;
		_alloc_ptr += bytes;
		_alloc_bytes_remaining -= bytes;
		return result;
	}
	char* Arena::AllocateAligned(size_t bytes)
	{
		const int align = sizeof(void*);
		assert((align&(align - 1)) == 0);//确保指针的大小为2的n次幂
		//einterpret_cast<type-id> (expression)
		//type - id 必须是一个指针、引用、算术类型、函数指针或者成员指针。
		//它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针
		//（先把一个指针转换成一个整数，再把该整数转换成原类型的指针，
		//还可以得到原先的指针值）。
		size_t current_mod = reinterpret_cast<uintptr_t>(_alloc_ptr) &
			(align - 1);
		size_t slop = (current_mod == 0 ? 0 : align - current_mod);
		size_t needed = bytes + slop;
		char* result;
		if (needed <= _alloc_bytes_remaining)
		{
			result = _alloc_ptr + slop;
			_alloc_ptr += needed;
			_alloc_bytes_remaining -= needed;
		}
		else
		{
			result = AllocateFallback(bytes);
		}
		assert((reinterpret_cast<uintptr_t>(result) & (align - 1)) == 0);
		return result;
	}
	char* Arena::AllocateNewBlock(size_t block_bytes)
	{
		char* result = new char[block_bytes];
		_block_memory += block_bytes;
		_blocks.push_back(result);
		return result;
	}
}