#pragma once
#ifndef _MYLEVELDB_PORT_WIN_H
#define _MYLEVELDB_PORT_WIN_H

//#define snprintf _snprintf
#define close _close
#define fread_unlocked _fread_nolock

#include <string>
#include <stdint.h>

namespace MyLevelDB
{
	namespace port
	{
		static const bool kLittleEndian = true;
		class CondVar;
		class Mutex
		{
		public:
			Mutex();
			~Mutex();

			void Lock();
			void Unlock();
			void AssertHeld();

		private:
			friend class CondVar;

			void* _cs;

			Mutex(const Mutex&) = delete;
			void operator=(const Mutex&) = delete;
		};
		class CondVar
		{
		public:
			explicit CondVar(Mutex*);
			~CondVar();
			void wait();
			void Signal();
			void SignalAll();
		private:
			Mutex* _mu;
			Mutex _wait_mtx;
			long _waiting;

			void* _sem1;
			void* _sem2;
		};

		class AtomicPointer
		{
		public:
			AtomicPointer():_rep(nullptr){}
			explicit AtomicPointer(void* v);
			void* Acquire_Load() const;

			void Release_Store(void*);
			
			void* NoBarrier_Load() const;

			void NoBarrier_Store(void*);
		private:
			void* _rep;
		};

		inline bool Snappy_Compress(const char* input, size_t length,
			::std::string* output)
		{
#ifdef SNAPPY
			output->resize(snappy::MaxCompressedLength(length));
			size_t outlen;
			snappy::RawCompress(input, length, &(*output)[0], &outlen);
			output->resize(outlen);
			return true;
#endif
			return false;
		}

		inline bool Snappy_GetUncompressedLength(const char* input, 
			size_t length,size_t* result)
		{
#ifdef SNAPPY
			return snappy::GetUncompressedLength(input, length, result);
#else
			return false;
#endif
		}
		inline bool Snappy_Uncompress(const char* input, size_t length,
			char* output)
		{
#ifdef SNAPPY
			return snappy::RawUncompress(input, length, output);
#else
			return false;
#endif // SNAPPY
		}
		inline bool GetHeapProfile(void (*func)(void*,const char*,int),void* arg)
		{
			return false;
		}
	}
}


#endif // !_MYLEVELDB_PORT_WIN_H