#pragma once
#include "Runtime/Plugins/al/al.h"
#include "Runtime/Utils/LinkedList.h"
#include "Runtime/IO/AliceData.h"
namespace Alice{
	class AudioBuffer : public LinkedList{
	public:
		AudioBuffer();
		virtual ~AudioBuffer();
		void SetPCMBufferSize(int buffer_size);
		Data mPCMData;
		float mStartTime;
		float mDuration;
		ALuint mBufferObject;
		void Reinit();
	};
}