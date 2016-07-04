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
		assert((align&(align - 1)) == 0);//ȷ��ָ��Ĵ�СΪ2��n����
		//einterpret_cast<type-id> (expression)
		//type - id ������һ��ָ�롢���á��������͡�����ָ����߳�Աָ�롣
		//�����԰�һ��ָ��ת����һ��������Ҳ���԰�һ������ת����һ��ָ��
		//���Ȱ�һ��ָ��ת����һ���������ٰѸ�����ת����ԭ���͵�ָ�룬
		//�����Եõ�ԭ�ȵ�ָ��ֵ����
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