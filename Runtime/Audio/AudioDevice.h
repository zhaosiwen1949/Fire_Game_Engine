#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Plugins/al/al.h"
#include "Runtime/Plugins/al/alc.h"
#include "AudioSource.h"
#include <set>
namespace Alice{
	class AudioDevice{
	public:
		AudioDevice();
		void Init();
		bool IsReady();
		void PlayBackgroundMusic(const char*pMusicFilePath);
		void Test(const char*pMusicFilePath);
		void PlayEffect(const char*pMusicFilePath);
		void Exit();
		void StopAllAudioSources();
		void OnNewAudioSource(AudioSource*as);
		void OnDeleteAudioSource(AudioSource*as);
		ALCdevice*mDevice;
		ALCcontext*mContext;
		static std::set<AudioSource*> mAudioSources;
	};
	AudioDevice *GetAudioDevice();
}