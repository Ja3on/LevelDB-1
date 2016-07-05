#include "../MyLevelDB/port_win.h"
#include <Windows.h>
#include <cassert>

namespace MyLevelDB
{
	namespace port
	{
		Mutex::Mutex()
			:_cs(nullptr)
		{
			assert(!_cs);
			_cs = static_cast<void*>(new CRITICAL_SECTION());
			::InitializeCriticalSection(static_cast<CRITICAL_SECTION*>(_cs));
			assert(_cs);
		}

		Mutex::~Mutex()
		{
			assert(_cs);
			::DeleteCriticalSection(static_cast<CRITICAL_SECTION*>(_cs));
			delete static_cast<CRITICAL_SECTION*>(_cs);
			_cs = nullptr;
			assert(!_cs);
		}

		void Mutex::Lock()
		{
			assert(_cs);
			::EnterCriticalSection(static_cast<CRITICAL_SECTION*>(_cs));
		}

		void Mutex::Unlock()
		{
			assert(_cs);
			::LeaveCriticalSection(static_cast<CRITICAL_SECTION*>(_cs));
		}

		void Mutex::AssertHeld()
		{
			assert(_cs);
			assert(1);
		}

		CondVar::CondVar(Mutex* mu)
			: _waiting(0),_mu(mu),
			_sem1(::CreateSemaphore(NULL,0,10000,NULL)),
			_sem2(::CreateSemaphore(NULL,0,10000,NULL))
		{
			assert(_mu);
		}
		CondVar::~CondVar()
		{
			::CloseHandle(_sem1);
			::CloseHandle(_sem2);
		}
		void CondVar::wait()
		{
			_mu->AssertHeld();
			_wait_mtx.Lock();
			++_waiting;
			_wait_mtx.Unlock();

			_mu->Unlock();

			::WaitForSingleObject(_sem1, INFINITE);
			::ReleaseSemaphore(_sem2, 1, NULL);
			_mu->Lock();
		}

		void CondVar::Signal()
		{
			_wait_mtx.Lock();
			if (_waiting > 0)
			{
				--_waiting;

				::ReleaseSemaphore(_sem1, 1, NULL);
				::WaitForSingleObject(_sem2, INFINITE);
			}
			_wait_mtx.Unlock();
		}

		void CondVar::SignalAll()
		{
			_wait_mtx.Lock();
			for (long i = 0; i < _waiting; ++i)
			{
				::ReleaseSemaphore(_sem1, 1, NULL);
				while (_waiting > 0)
				{
					--_waiting;
					::WaitForSingleObject(_sem2, INFINITE);
				}
			}
			_wait_mtx.Unlock();
		}
		AtomicPointer::AtomicPointer(void* v)
		{
			Release_Store(v);
		}
		void* AtomicPointer::Acquire_Load() const
		{
			void* p = nullptr;
			InterlockedExchangePointer(&p, _rep);
			return p;
		}
		void AtomicPointer::Release_Store(void* v)
		{
			InterlockedExchangePointer(&_rep, v);
		}
		void* AtomicPointer::NoBarrier_Load() const
		{
			return _rep;
		}
		void AtomicPointer::NoBarrier_Store(void* v)
		{
			_rep = v;
		}
	}
}