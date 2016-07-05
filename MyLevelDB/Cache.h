#pragma once
#ifndef _MYLEVELDB_CACHE_H
#define _MYLEVELDB_CACHE_H

#include <stdint.h>
#include "../MyLevelDB/Slice.h"
#include "../MyLevelDB/port.h"

namespace MyLevelDB
{
	class Cache;
	extern Cache* NewLRUCache(size_t);

	class Cache
	{
	public:
		Cache(){}
		struct Handle{};

		virtual Handle* Insert(const Slice&, void*, size_t,
			void(*deleter)(const Slice&, void*)) = 0;

		virtual Handle* Lookup(const Slice& key) = 0;
		virtual void Release(Handle*) = 0;
		virtual void* Value(Handle*) = 0;
		virtual void Erase(const Slice&) = 0;
		virtual uint64_t NewId() = 0;

	private:
		void _LRU_Remove(Handle*);
		void _LRU_Append(Handle*);
		void _Unref(Handle*);

		struct Rep;
		Rep* _rep;
		Cache(const Cache&) = delete;
		void operator=(const Cache&) = delete;
	};
}

#endif // !_MYLEVELDB_CACHE_H