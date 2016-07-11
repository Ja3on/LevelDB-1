#pragma once
#ifndef _MYLEVELDB_MUTEXLOCK_H
#define _MYLEVELDB_MUTEXLOCK_H

#include "../MyLevelDB/port_win.h"
#include "../MyLevelDB/port.h"

namespace MyLevelDB
{
	class MutexLock
	{
	public:
		explicit MutexLock(port::Mutex *mu) :_mu(mu)
		{
			this->_mu->Lock();
		}

		~MutexLock() { this->_mu->Unlock(); }

	private:
		port::Mutex *const _mu;

		MutexLock(const MutexLock&) = delete;
		void operator(const MutexLock&) = delete;
	};
}

#endif // !_MYLEVELDB_MUTEXLOCK_H