#include "Timer.h"
#include "Runtime/Debugger/log.h"

namespace Alice
{
#if _WIN32
	Timer::Timer() : m_startTime(0), m_threadHandle(::GetCurrentThread()){
		DWORD_PTR systemMask;
		::GetProcessAffinityMask(GetCurrentProcess(), &m_processAffinityMask, &systemMask);
		::SetThreadAffinityMask(m_threadHandle, 1);
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
		::SetThreadAffinityMask(m_threadHandle, m_processAffinityMask);
	}
	
	void Timer::Start()
	{
		m_startTime = GetTime();
	}

	float Timer::GetTimeInMs() const
	{
		if (m_startTime ==0)
		{
			return 0.0f;
		}
		__int64 const elapsedTime = GetTime() - m_startTime;
		double const seconds = double(elapsedTime) / double(m_frequency);
		return (float)(seconds * 1000.0f);
	}

	AliceUInt64 Timer::GetTimeInMsi()
	{
		__int64 const elapsedTime = GetTime() - m_startTime;
		double const seconds = double(elapsedTime) / double(m_frequency);
		return AliceUInt64(seconds * 1000.0f);
	}

	void Timer::Reset()
	{
		Start();
	}

	__int64 Timer::GetTime() const
	{
		LARGE_INTEGER curTime;
		::SetThreadAffinityMask(m_threadHandle, 1);
		::QueryPerformanceCounter(&curTime);
		::SetThreadAffinityMask(m_threadHandle, m_processAffinityMask);
		return curTime.QuadPart;
	}

#elif ALICE_IPHONE || ALICE_ANDROID || ALICE_OSX_PLAYER

    Timer::Timer()
    {
        
    }
    
    void Timer::Start()
    {
        gettimeofday(&mStartTime,nullptr);
        mStartTimeInMS=((AliceUInt64)mStartTime.tv_sec)*1000+(AliceUInt64)mStartTime.tv_usec/1000;
    }
    
    AliceUInt64 Timer::GetTimeInMsi()
    {
        timeval end;
        gettimeofday(&end,nullptr);
        AliceUInt64 endTimeInMS=((AliceUInt64)end.tv_sec)*1000+(AliceUInt64)end.tv_usec/1000;
        return endTimeInMS-mStartTimeInMS;
    }

	float Timer::GetTimeInMs() const
	{
		timeval end;
		gettimeofday(&end, nullptr);
		AliceUInt64 endTimeInMS = ((AliceUInt64)end.tv_sec) * 1000 + (AliceUInt64)end.tv_usec / 1000;
        return (float)(endTimeInMS - mStartTimeInMS);///1000.0f;
	}
    void Timer::Reset()
    {
        Start();
    }
#endif
}
