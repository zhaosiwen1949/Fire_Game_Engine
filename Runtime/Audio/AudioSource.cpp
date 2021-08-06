#include "AudioSource.h"
#include "AudioDebugger.h"
#include "Runtime/Scene/GameObject.h"
#include "AudioDevice.h"

namespace Alice{
	AudioSource::AudioSource():mbLoop(false),mAudioResource(nullptr){
		mState = kAudioSourceStopped;
		mAudioPath.Resize(256);
		mOnStopCallback = LUA_REFNIL;
		mAudioSourceInstance = 0;
		mVolume = 1.0f;
		GetAudioDevice()->OnNewAudioSource(this);
		mAudioTime = 0.0f;
		mHeadAudioBuffer = nullptr;
	}
	AudioSource::~AudioSource(){
		DeletePrevInstance();
		CleanUnprocessedBuffers();
		GetAudioDevice()->OnDeleteAudioSource(this);
	}
	void AudioSource::InitWithAudioResource(AudioResource*audioResource)
	{
		mAudioResource = audioResource;
		mAudioTime = 0.0f;
		mQueuedBufferSize = 0;
	}

	void AudioSource::PrepareFrom(int second) {
		if (mAudioResource==nullptr){
			return;
		}
        Stop();
		NewAudioInstance();
		int nFrame_index=mAudioResource->SetAudioPosBySecond(second);
		mAudioTime = nFrame_index * 0.026f;
	}

	void AudioSource::StreamAudioBuffer() {
		while (mQueuedBufferSize<10){
			AudioBuffer*ab = new AudioBuffer;
			ab->SetPCMBufferSize(10240);
			int ret = mAudioResource->GetNextData(2, ab->mPCMData);
			if (ret < 0 || ab->mPCMData.mDataLen==0) {
				delete ab;
				return ;
			}
			ab->mDuration = 0.026f * 2;
			ApplyVolume(ab->mPCMData.mData, ab->mPCMData.mDataLen);
			ab->Reinit();
			OAL_CALL(alBufferData(ab->mBufferObject, mAudioResource->mFormat, ab->mPCMData.mData, ab->mPCMData.mDataLen, mAudioResource->mFrequence));
			OAL_CALL(alSourceQueueBuffers(mAudioSourceInstance, 1, &ab->mBufferObject));
			if (mHeadAudioBuffer!=nullptr){
				mHeadAudioBuffer->PushBack(ab);
			}else {
				mHeadAudioBuffer = ab;
			}
			++mQueuedBufferSize;
		}
	}

	void AudioSource::CleanUnprocessedBuffers() {
		int i = 0;
		while (mHeadAudioBuffer != nullptr) {
			AudioBuffer*current = mHeadAudioBuffer;
			mHeadAudioBuffer = mHeadAudioBuffer->Next<AudioBuffer>();
			++i;
			delete current;
		}
	}
	//OAL_CALL(alSourcef(mAudioSourceInstance, AL_PITCH, 2.0f));��Ϳ��Լ�����Ч�Ĳ���
	//Ȼ����֡�����Լ��ֶ����ƾ���~������ʵ�ֱ��ٲ���
	void AudioSource::NewAudioInstance() {
		DeletePrevInstance();
		//clear unprocessed audio buffer
		CleanUnprocessedBuffers();
		mQueuedBufferSize = 0;
		OAL_CALL(alGenSources(1, &mAudioSourceInstance));
		SetVolume(mVolume);
	}
	void AudioSource::DeletePrevInstance() {
		if (mAudioSourceInstance != 0) {
			OAL_CALL(alDeleteSources(1, &mAudioSourceInstance));
			mAudioSourceInstance = 0;
		}
	}

	void AudioSource::SetAudioResource(const char*name){
		if (name !=nullptr){
			AudioResource*audioResource = Resource::LoadResource<AudioResource>(name);
			if (audioResource != nullptr){
				mAudioPath = name;
				InitWithAudioResource(audioResource);
			}else{
				errorC("SetAudioResource fail %s", name);
			}
		}else {
			errorC("SetAudioResource,name is nullptr");
		}
	}

	void AudioSource::SetPosition(float x, float y, float z)
	{
		OAL_CALL(alSource3f(mAudioSourceInstance,AL_POSITION,x,y,z));
	}

	void AudioSource::SetVolume(float v) {
		mVolume = v > 5.0f ? 5.0f:v;
		if (mAudioSourceInstance != 0) {
			OAL_CALL(alSourcef(mAudioSourceInstance, AL_GAIN, v));
		}
	}
	void AudioSource::ApplyVolume(unsigned char *data, int len) {
		if (mVolume > 1.0f) {
			int dataCount = len / 2;
			short *ad = (short*)data;
			for (int i = 0; i < dataCount; ++i) {
				int temp = ad[i];
				temp = int(temp*mVolume);
				if (temp > 32767) {
					temp = 32767;
				}
				if (temp < -32767)
				{
					temp = -32767;
				}
				ad[i] = temp;
			}
		}
	}
	void AudioSource::Update(float deltaTime){
		if (mAudioSourceInstance==0){
			return;
		}
		if (mState!=kAudioSourcePlaying&&mState!=kAudioSourceWaittingForData){
			return;
		}
		//��鵱ǰ��Ƶ���ݶ��е��������Ҫ��Ҫ���Ѿ����Ź���buffer���������
		//����ϵͳ���ٶ���Ƶϵͳ���Ӱ��
		CheckCurrentBufferState();
		StreamAudioBuffer();
		//Debug("audio time %f",mAudioTime);
		ALint curret_state = AL_NONE;
		OAL_CALL(alGetSourcei(mAudioSourceInstance, AL_SOURCE_STATE, &curret_state));
		//�����ǰ��״̬���ڲ���״̬����ô�ͽ��벥���߼�
		if (mState==kAudioSourcePlaying){
			if (curret_state != AL_PLAYING) {
				if (mQueuedBufferSize > 0) {
					OAL_CALL(alSourcePlay(mAudioSourceInstance));
				}
				else {
					OnStop();//������ϻ��ߵȴ��������ݣ���OnStop�л��ж�
				}
			}
		//�����ǰ��״̬���ڵȴ�����״̬����ô�ͽ���ȴ����ݵ��߼�
		}else if (mState == kAudioSourceWaittingForData) {
			//������ڵȴ����ݵ�״̬�����ҵ�ǰ��buffer�Ѿ����ھ���״̬��˵����������
			//�����״̬Ϊ����״̬�����Ҹ���һ���ص������߽ű��㣬�ӵȴ�ת�벥��״̬
			if (mQueuedBufferSize>0) {
				OAL_CALL(alSourcePlay(mAudioSourceInstance));
				mState = kAudioSourcePlaying;
				mOwner->Call("OnResumeFromWaitData", nullptr);
			}
		}
	}


	void AudioSource::CheckCurrentBufferState() {
		ALint ret = 0;
		OAL_CALL(alGetSourcei(mAudioSourceInstance, AL_BUFFERS_PROCESSED, &ret));//query how many buffer is inside the queue
		if (ret>0){
			for (int i = 0; i < ret; ++i) {
				AudioBuffer*current = mHeadAudioBuffer;
				mHeadAudioBuffer = mHeadAudioBuffer->Next<AudioBuffer>();
				mAudioTime += current->mDuration;
				OAL_CALL(alSourceUnqueueBuffers(mAudioSourceInstance, 1, &current->mBufferObject));
				delete current;
			}
			mQueuedBufferSize -= ret;
		}
	}
	bool AudioSource::IsPlaying()
	{
		return mState == kAudioSourcePlaying;
	}

	void AudioSource::Play(){
		if (mState == kAudioSourceStopped ||
			mState == kAudioSourcePaused){
			mState = kAudioSourcePlaying;
			OAL_CALL(alSourcePlay(mAudioSourceInstance));//�����Դ��û���κ�buffer��play֮�����AL_STOPPED״̬
		}
	}

	void AudioSource::Pause()
	{
		if (mState == kAudioSourcePlaying){
			mState = kAudioSourcePaused;
			OAL_CALL(alSourcePause(mAudioSourceInstance));
		}
	}

	void AudioSource::Stop()
	{
		if (mState == kAudioSourcePaused||
			mState == kAudioSourcePlaying){
			mState = kAudioSourceStopped;
			OAL_CALL(alSourceStop(mAudioSourceInstance));
		}
	}

	void AudioSource::OnStop(){
		if (mState==kAudioSourcePlaying){
			if (mAudioResource->mbCached) {
				mState = kAudioSourceStopped;
				mOwner->Call("OnAudioSourceStop", nullptr);
			} else {
				mState = kAudioSourceWaittingForData;
				mOwner->Call("OnAudioWaitForData", nullptr);
			}
		}
	}

	void AudioSource::SetLoop(bool bLoop)
	{
		mbLoop = bLoop;
	}

}
