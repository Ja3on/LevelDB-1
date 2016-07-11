#include "../MyLevelDB/Cache.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../MyLevelDB/Cache.h"
#include "../MyLevelDB/port_win.h"
#include "../MyLevelDB/port.h"
#include "../MyLevelDB/hash.h"
#include "../MyLevelDB/mutexlock.h"

namespace MyLevelDB
{
	Cache::~Cache() {}
	namespace
	{
		struct LRUHandle
		{
			void* value;
			void(*deleter)(const Slice&, void* value);
			LRUHandle* next_hash;
			LRUHandle* next;
			LRUHandle* prev;
			size_t charge;
			size_t key_length;
			uint32_t refs;
			uint32_t hash;
			char key_data[1];

			Slice key() const
			{
				if (next == this)
					return *(reinterpret_cast<Slice*>(value));
				else return Slice(key_data, key_length);
			}
		};

		class HandleTable
		{
		public:
			HandleTable() :_length(0), _elems(0), _list(NULL) { Resize(); }
			~HandleTable() { delete[] _list; }

			LRUHandle* Lookup(const Slice& key, uint32_t hash)
			{
				return *FindPointer(key, hash);
			}
			LRUHandle* Insert(LRUHandle* h)
			{
				LRUHandle** ptr = FindPointer(h->key(), h->hash);
				LRUHandle* old = *ptr;
				h->next_hash = (old == NULL ? NULL : old->next_hash);
				*ptr = h;
				if (old == NULL)
				{
					++_elems;
					if (_elems > _length) Resize();
				}
				return old;
			}

			LRUHandle* Remove(const Slice& key, uint32_t hash)
			{
				LRUHandle** ptr = FindPointer(key, hash);
				LRUHandle* result = *ptr;
				if (result != NULL)
				{
					*ptr = result->next_hash;
					--_elems;
				}
				return result;
			}

			
			
		private:
			uint32_t _length;
			uint32_t _elems;
			LRUHandle** _list;

			LRUHandle** FindPointer(const Slice& key, uint32_t hash)
			{
				LRUHandle** ptr = &_list[hash&(_length - 1)];
				while (*ptr != NULL &&
					((*ptr)->hash != hash || key != (*ptr)->key()))
					ptr = &(*ptr)->next_hash;
				return ptr;
 			}

			void Resize()
			{
				uint32_t new_length = 4;
				while (new_length < _elems) new_length *= 2;
				LRUHandle** new_list = new LRUHandle*[new_length];
				memset(new_list, 0, sizeof(new_list[0])*new_length);
				uint32_t count = 0;
				for (uint32_t i = 0; i < _length; ++i)
				{
					LRUHandle* h = _list[i];
					while (h != NULL)
					{
						LRUHandle* next = h->next_hash;
						Slice key = h->key();
						uint32_t hash = h->hash;
						LRUHandle** ptr = &new_list[hash&(new_length - 1)];
						h->next_hash = *ptr;
						*ptr = h;
						h = next;
						++count;
					}
				}
				assert(_elem == count);
				delete[] _list;
				_list = new_list;
				_length = new_length;
			} 

		};


	} //end of anonymous namespace
}
