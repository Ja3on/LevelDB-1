#pragma once
#ifndef _MYLEVELDB_STATUS_H
#define _MYLEVELDB_STATUS_H

#include <string>
#include "../MyLevelDB/Slice.h"

namespace MyLevelDB
{
	class Status
	{
	public:
		Status():_state(NULL){}
		~Status() { delete[] _state; }

		Status(const Status&);
		void operator=(const Status&);

		static Status OK() { return Status(); }
		static Status NotFound(const Slice& msg,const Slice& msg2=Slice())
		{
			return Status(kNotFound, msg, msg2);
		}
		static Status Corruption(const Slice& msg, const Slice& msg2 = Slice())
		{
			return Status(kCorruption, msg, msg2);
		}
		static Status NotSupported(const Slice& msg, const Slice& msg2)
		{
			return Status(kNotSupported, msg, msg2);
		}
		static Status InvalidArgument(const Slice& msg, const Slice& msg2)
		{
			return Status(kInvalidArgument, msg, msg2);
		}
		static Status IoError(const Slice& msg, const Slice& msg2)
		{
			return Status(kIoError, msg, msg2);
		}

		bool ok() const { return (_state == NULL); }
		bool IsNotFound() const { return code() == kNotFound; }
		std::string ToString() const;

	private:
		//_state[0...3] == length of message
		//_state[4] == Code
		//_state[5...]==message
		const char* _state;

		enum Code
		{
			kOk = 0,
			kNotFound = 1,
			kCorruption = 2,
			kNotSupported = 3,
			kInvalidArgument = 4,
			kIoError = 5
		};
		Code code() const
		{
			return (_state == NULL) ? kOk : static_cast<Code>(_state[4]);
		}
		Status(Code code, const Slice& msg, const Slice& msg2);
		static const char* CopyState(const char*);
	};

	inline Status::Status(const Status& s)
	{
		_state = (s._state == NULL) ? NULL : CopyState(s._state);
	}
	inline void Status::operator=(const Status& rhs)
	{
		if (_state != rhs._state)
		{
			delete[] _state;
			_state = (rhs._state == NULL) ? NULL : CopyState(rhs._state);
		}

	}
}

#endif // !_MYLEVELDB_STATUS_H