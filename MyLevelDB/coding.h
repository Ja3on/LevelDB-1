#pragma once
#ifndef _MYLEVELDB_CODING_H
#define _MYLEVELDB_CODING_H

#include <stdint.h>
#include <string>
#include <string.h>
#include "../MyLevelDB/Slice.h"
#include "../MyLevelDB/port.h"
#include "../MyLevelDB/port_win.h"

namespace MyLevelDB
{
	extern void PutFixed32(std::string*, uint32_t);
	extern void PutFixed64(std::string*, uint64_t);
	extern void PutVarint32(std::string*, uint32_t);
	extern void PutVarint64(std::string*, uint64_t);
	extern void PutLengthPrefixedSlice(std::string*, const Slice&);

	extern bool GetVarint32(Slice*, uint32_t*);
	extern bool GetVarint64(Slice*, uint64_t*);
	extern bool GetLengthPrefixedSlice(Slice*, Slice*);

	extern const char* GetVarint32Ptr(const char*, const char*, uint32_t*);
	extern const char* GetVarint64Ptr(const char*, const char*, uint64_t*);

	extern int VarintLength(uint64_t);

	extern void EncodeFixed32(char*, uint32_t);
	extern void EncodeFixed64(char*, uint64_t);

	extern char* EncodeVarint32(char*, uint32_t);
	extern char* EncodeVarint64(char*, uint64_t);

	inline uint32_t DecodeFixed32(const char* ptr)
	{
		if (port::kLittleEndian)
		{
			uint32_t result;
			memcpy(&result, ptr, sizeof(result));
			return result;
		}
		else
		{
			return (static_cast<uint32_t>(ptr[0]))
				| (static_cast<uint32_t>(ptr[1]) << 8)
				| (static_cast<uint32_t>(ptr[2]) << 16)
				| (static_cast<uint32_t>(ptr[3]) << 24);
		}
	}
	inline uint64_t DecodeFixed64(const char* ptr)
	{
		if (port::kLittleEndian)
		{
			uint64_t result;
			memcpy(&result, ptr, sizeof(result));
			return result;
		}
		else
		{
			uint64_t lo = DecodeFixed32(ptr);
			uint64_t hi = DecodeFixed32(ptr + 4);
			return (hi << 32) | lo;
		}
	}

	extern const char* GetVarint32PtrFallback(const char* p, const char* limit,
		uint32_t* value);

	inline const char* GetVarint32Ptr(const char* p, const char* limit,
		uint32_t* value)
	{
		if (p < limit)
		{
			uint32_t result = *(reinterpret_cast<const unsigned char*>(p));
			if ((result & 128) == 0)
			{
				*value = result;
				return p + 1;
			}
		}
		return GetVarint32PtrFallback(p, limit, value);
	}

}

#endif // !_MYLEVELDB_CODING_H