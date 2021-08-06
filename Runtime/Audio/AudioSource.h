#pragma once
#include "Runtime/Plugins/Lua/lua.hpp"
#include "Runtime/Plugins/al/al.h"
#include "Runtime/Serializer/Audio.serializer.h"
#include "Runtime/IO/AudioResource.h"
#include "Runtime/Base/Component.h"
#include "Runtime/String/FixedString.h"
#include "Runtime/Utils/SmartPtr.h"
#include "AudioBuffer.h"
namespace Alice{
	class AudioSource:public Component{
	public:
		enum AudioSourceState{
			kAudioSourceStopped,
			kAudioSourcePlaying,
			kAudioSourcePaused,
			kAudioSourceWaittingForData,
			kAudioSourceStateCount
		};
		FixedString mAudioPath;
		AudioSource();
		virtual ~AudioSource();
		void Play();
		bool IsPlaying();
		void Pause();
		void Stop();
		void SetVolume(float v);
		void OnStop();
		void SetLoop(bool bLoop);
		void InitWithAudioResource(AudioResource*audioResource);
		void PrepareFrom(int second);
		OPT(void) SetAudioResource(const char*name);
		void SetPosition(float x,float y,float z);
		virtual void Update(float deltaTime);
		bool mbLoop;
		AudioSourceState mState;
		ALuint mAudioSourceInstance;
		int mOnStopCallback;
		AudioBuffer*mHeadAudioBuffer;
		int mQueuedBufferSize;
		float mVolume;
		float mAudioTime;
		SmartPtr<AudioResource> mAudioResource;
	protected:
		void CheckCurrentBufferState();
		void DeletePrevInstance();
		void CleanUnprocessedBuffers();
		void NewAudioInstance();
		void StreamAudioBuffer();
		void ApplyVolume(unsigned char *data,int len);
	public:
		DECLEAR_ALICE_CLASS(AudioSource)
	};
}
