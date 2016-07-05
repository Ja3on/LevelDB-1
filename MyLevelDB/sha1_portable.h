#pragma once
#ifndef _MYLEVELDB_SHA1_PORTABLE_H
#define _MYLEVELDB_SHA1_PORTABLE_H

#include <stddef.h>

namespace MyLevelDB
{
	namespace port
	{
		//SHA1 hash °²È«¹þÏ£Ëã·¨
		void SHA1_Hash_Portable(const char*, size_t, char*);
	}
}

#endif // !_MYLEVELDB_SHA1_PORTABLE_H