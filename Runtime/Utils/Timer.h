#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Base/Object.h"
#include "Runtime/NetWork/SocketUtils.h"
#ifdef ALICE_PLATFORM_WIN
//sleep for ms
#define AliceSleep(ms) Sleep(ms)
#elif ALICE_PLATFORM_UNIX
#include <sys/time.h>
#define AliceSleep(ms) CanRead(0,0,ms)
#endif

namespace Alice{
	class Timer : public Object{
	public:
		Timer();
		void Start();
		float GetTimeInMs() const;
		AliceUInt64 GetTimeInMsi();
		void Reset();
	private:
#ifdef ALICE_PLATFORM_WIN
		__int64 GetTime() const;
		void* m_threadHandle;
		unsigned long m_processAffinityMask;
		__int64 m_startTime;
		__int64 m_frequency;
#elif ALICE_PLATFORM_UNIX
        timeval mStartTime;
        AliceUInt64 mStartTimeInMS;
#endif
	};
}
