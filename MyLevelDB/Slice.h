#pragma once
#ifndef _MYLEVELDB_Slice_H
#define _MYLEVELDB_Slice_H

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <string>

namespace MyLevelDB
{
	class Slice
	{
	public:
		Slice() :_data(""), _size(0) {}
		Slice(const char* data, size_t n) :_data(data), _size(n) {}
		Slice(const std::string& s) :_data(s.data()), _size(s.size()) {}
		Slice(const char* s) :_data(s), _size(strlen(s)) {}

		const char* data() const { return _data; }
		size_t size() const { return _size; }
		bool empty() const { return _size == 0; }

		char operator[](size_t n) const
		{
			assert(n < size());
			return _data[n];
		}

		void clear() { _data = ""; _size = 0; }

		void remove_prefix(size_t n)
		{
			assert(n < size());
			_data += n;
			_size -= n;
		}

		std::string ToString() const { return std::string(_data, _size); }

		int compare(const Slice&) const;

		bool starts_with(const Slice& x) const
		{
			return ((_size >= x._size) &&
				memcmp(_data, x._data, x._size) == 0);
		}

	private:
		const char* _data;
		size_t _size;
	};

	inline bool operator==(const Slice& lhs, const Slice& rhs)
	{
		return ((lhs.size() == rhs.size()) &&
			memcmp(lhs.data(), rhs.data(), rhs.size()) == 0);
	}
	inline bool operator!=(const Slice& lhs, const Slice& rhs)
	{
		return !(lhs == rhs);
	}
	inline int Slice::compare(const Slice& rhs) const
	{
		const int min_len = (_size < rhs._size ? _size : rhs._size);
		int r = memcmp(_data, rhs._data, min_len);
		if (r == 0)
		{
			if (_size < rhs._size) r = -1;
			else if (_size > rhs._size) r = 1;
		}
		return r;
	}
}

#endif // !_MYLEVELDB_Slice_H