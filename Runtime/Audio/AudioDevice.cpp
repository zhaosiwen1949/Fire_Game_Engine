#include "AudioDevice.h"
#include <stdio.h>
#include <string.h>
#include "Runtime/Plugins/al/al.h"
#include "Runtime/Plugins/al/alc.h"
#if ALICE_PLATFORM_WIN
#pragma comment(lib,"OpenAL32.lib")
#endif
#include "AudioDebugger.h"
#include "AudioSource.h"

#if ALICE_EDITOR
#define fileno _fileno
#endif

namespace Alice{
	static AudioDevice *sAudioDevice=nullptr;
#if ALICE_EDITOR
	static mpg123_handle *mh;
	static ALuint sSourceID;
	static ALuint sEffect;
#endif
	AudioDevice*GetAudioDevice(){
		if (nullptr==sAudioDevice){
			sAudioDevice = new AudioDevice;
			sAudioDevice->Init();
		}
		return sAudioDevice;
	}

	AudioDevice::AudioDevice():mDevice(nullptr),mContext(nullptr){
	}

	bool getInfo(ALenum &format, ALsizei &freq, ALsizei &size)
	{
#if ALICE_EDITOR
		int channels = 0;
		int encoding = 0;
		long rate = 0;
		if (MPG123_OK != mpg123_getformat(mh, &rate, &channels, &encoding))
			return false;
		size = mpg123_length(mh);
		if (size == MPG123_ERR)
			return false;
		freq = rate;
		if (encoding == MPG123_ENC_UNSIGNED_8) {
			if (channels == 1)
				format = AL_FORMAT_MONO8;
			else
				format = AL_FORMAT_STEREO8;
		}
		else {
			if (channels == 1)
				format = AL_FORMAT_MONO16;
			else
				format = AL_FORMAT_STEREO16;
		}
#endif
		return true;
	}
	void AudioDevice::Init()
	{
#if ALICE_EDITOR
		int err;
		mpg123_init();
		mh = mpg123_new(nullptr, &err);
		if (MPG123_OK != mpg123_format(mh, 44100, MPG123_MONO | MPG123_STEREO,
			MPG123_ENC_UNSIGNED_8 | MPG123_ENC_SIGNED_16))
		{
			printf("fucked mpg123_format\n");
		}
#endif
		OAL_CALL(mDevice = alcOpenDevice(nullptr));
		if (mDevice){
			mContext = alcCreateContext(mDevice,nullptr);
			OAL_CALL(alcMakeContextCurrent(mContext));//Android和Windows、OSX上的初始化不会独占喇叭。iOS端会
			Info("Open Audio Device Sucess!");
		}else{
			Error("create openal device fail");
			return;
		}
#if ALICE_EDITOR
		OAL_CALL(alGenSources(1, &sSourceID));
		OAL_CALL(alGenSources(1, &sEffect));
#endif
	}

	bool AudioDevice::IsReady() {
		return mDevice != nullptr;
	}

	void AudioDevice::PlayBackgroundMusic(const char*pMusicFilePath)
	{
		ALuint audioBuffer;
		ALuint effectBuffer;
		OAL_CALL(alGenBuffers(1,&audioBuffer));
		OAL_CALL(alGenBuffers(1, &effectBuffer));
#if ALICE_EDITOR
		FILE* pFile = fopen(pMusicFilePath, "rb");
		//char *mp3FileData = LoadFile("SwomeeSwan.mp3", mp3FileLen);
		mpg123_param(mh, MPG123_VERBOSE, 2, 0);
		//mpg123_open_feed(mh);
		//检测格式是否正确
		//mpg123_open(mh, "SwomeeSwan.mp3");
		//mpg123_getformat(mh, &rate, &channels, &enc);

		//if(enc != MPG123_ENC_SIGNED_16 && enc != MPG123_ENC_FLOAT_32) 
		{
			//	fprintf(stderr, "Bad encoding: 0x%x!\n", enc);
			//return;
		}
		if (MPG123_OK != mpg123_open_fd(mh, fileno(pFile)))
		{
			printf("fucked MPG123_OK != mpg123_open_fd(mh, fileno(pFile))");
		}
		//int ret=mpg123_decode(mh, (const unsigned char*)mp3FileData, mp3FileLen, (unsigned char *)buffer, bufferSize, &done);
		ALenum format = AL_NONE;
		ALsizei freq = 0;
		ALsizei size = 0;
		size_t done = 0;
		size_t rSize = 0;
		if (!getInfo(format, freq, size))
		{
			printf("fucked !getInfo(format, freq, size)");
		}
		rSize = size;
		if (format == AL_FORMAT_MONO16 || format == AL_FORMAT_STEREO16)
			rSize *= 2;
		long n = mpg123_length(mh) * 16 * 2 / 8;
		unsigned char *data = new unsigned char[n];
		memset(data,0, n);
		int err = mpg123_read(mh, data, n, &done);
		if (err==MPG123_ERR)
		{
			printf("fucked MPG123_DONE != mpg123_read(mh, (unsigned char*)data, size, &done)\n");
		}
		if ((err == MPG123_DONE) || (err == MPG123_OK))
		{
			printf("(err == MPG123_DONE) || (err == MPG123_OK)\n");
		}
		printf("MP3 BUFFER SIZE: %ld, FORMAT %i\n", (long)done, (int)format);
		if (done)
		{
			if (alIsBuffer(audioBuffer)==AL_TRUE)
			{
				printf("fucked is a buffer\n");
			}
			alBufferData(audioBuffer, format, data, done, freq);
			alBufferData(effectBuffer, format, data, done, freq);
			alSourcei(sSourceID, AL_BUFFER, audioBuffer);
			alSourcei(sEffect, AL_BUFFER, effectBuffer);
		}
		ALint values;
		alGetSourcei(sSourceID, AL_SOURCE_STATE, &values);
		if (values != AL_PLAYING)
		{
			alSourcePlay(sSourceID);
			Sleep(1000);
			alSourcePlay(sEffect);
		}
		alGetSourcei(sSourceID, AL_SOURCE_STATE, &values);
		//0.0~1.0
		alSourcef(sSourceID, AL_GAIN, 1.0f);
		if (values != AL_PLAYING)
		{
			alSourcePlay(sSourceID);
		}
#endif
		//alDeleteBuffers(1, &bufferID);
	}

	void AudioDevice::Test(const char*pMusicFilePath)
	{
		//AudioClip *clip=AudioClip::InitWithAudioFile(pMusicFilePath);
		//AudioSource *audioSource=new AudioSource;
		//audioSource->SetAudioClip(clip);
		//audioSource->Play();
	}

	void AudioDevice::PlayEffect(const char*pMusicFilePath)
	{

	}

	void AudioDevice::Exit()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);
		mDevice = nullptr;
		mContext = nullptr;
	}
	std::set<AudioSource*> AudioDevice::mAudioSources;
	void AudioDevice::StopAllAudioSources()
	{
		for (std::set<AudioSource*>::iterator iter=mAudioSources.begin();iter!=mAudioSources.end();++iter)
		{
			if ((*iter)->IsPlaying())
			{
				(*iter)->Stop();
			}
		}
	}

	void AudioDevice::OnNewAudioSource(AudioSource*as){
		mAudioSources.insert(as);
	}

	void AudioDevice::OnDeleteAudioSource(AudioSource*as){
		auto iter = mAudioSources.find(as);
		if (iter!= mAudioSources.end()){
			mAudioSources.erase(iter);
		}
	}
}
