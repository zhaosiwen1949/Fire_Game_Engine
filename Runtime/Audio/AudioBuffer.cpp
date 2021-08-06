#include "AudioBuffer.h"
#include "AudioDebugger.h"
#include "Runtime/Scene/GameObject.h"
#include "AudioDevice.h"

namespace Alice {
	AudioBuffer::AudioBuffer() {
		mBufferObject = 0;
		mStartTime = -1.0f;
		mDuration = 0.0f;
	}
	AudioBuffer::~AudioBuffer() {
		if (mBufferObject != 0) {
			OAL_CALL(alDeleteBuffers(1, &mBufferObject));
			mBufferObject = 0;
		}
	}
	void AudioBuffer::Reinit() {
		if (mBufferObject != 0) {
			OAL_CALL(alDeleteBuffers(1, &mBufferObject));
			mBufferObject = 0;
		}
		OAL_CALL(alGenBuffers(1, &mBufferObject));
	}
	void AudioBuffer::SetPCMBufferSize(int buffer_size) {
		mPCMData.SetBufferSize(buffer_size);
	}
}
