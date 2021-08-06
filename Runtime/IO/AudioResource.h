#pragma once
#include "AliceResource.h"
#include "Runtime/Utils/LinkedList.h"
#include "Runtime/Plugins/al/al.h"
#if ALICE_WIN_PLAYER || ALICE_EDITOR
#include "Runtime/Plugins/mpg123/mpg123.h"
#elif ALICE_OSX_PLAYER || ALICE_IPHONE
#include "Runtime/Plugins/mpg123/mpg123osx.h"
#elif ALICE_ANDROID
#include "Runtime/Plugins/mpg123/mpg123android.h"
#endif
#include "Runtime/Plugins/id3/id3v2lib.h"
namespace Alice{
	enum AudioResourceFormat{
		AudioResourceFormatMP3=1,
		AudioResourceFormatPCM=2
	};
	class MP3Frame : public LinkedList {
	public:
		int mFrameIndex;
		//MP3Frameԭʼ���ݣ�ָ��MP3File�Ļ��������Լ����׵�ַ
		unsigned char *mMP3FrameData;
		int mFrameSizeInByte;
	};
	class AudioResource:public Resource{
	public:
        AudioResource();
        ~AudioResource();
		AliceUInt8 *mRawData;
		AliceSInt32 mRawDataBufferSize;
		AliceSInt32 mCachedRawDataSize;
		AliceSInt32 mFrequence;
		ALenum mFormat;
		int mChannelCount;
		int mEncoding;
		mpg123_handle * mHandle;
		ID3v2_tag *mID3Tag;
		float mTPF;
		MP3Frame*mHeadFrame;
		MP3Frame*mCurrentFrame;
		MP3Frame*mEndFrame;
		int mCurrentFrameIndex;
		int mFrameCount;
		bool mbCached;
		//��ʼ��MP3�ļ��ķ�ʽΪ����ʼ��ID3ͷ����Ȼ���ٿ�ʼ����������Ƶ֡��
		//Ŀǰ������Ժֻ֧��ID3���ļ�ͷ�������������ID3���ļ�ĩβ������������
		bool TryToInitHeader();
		//���˳�ʼ��MP3 ID3ͷ�����⣬����Ҫ�����ǰ8֡�����ݣ��Ի��MP3����Ϣ
		//Ҳ����MPG123_NEW_FORMAT���õ���һ�����ء��Ӵ˿̿�ʼ���Ϳ��԰���֡���������ݡ�
		//��ͨ���Ͷ�ͨ��������������
		bool TryToGetMP3Info();
		void ScanMP3FileData();
		void SetCurrentFrame(int nFrameIndex);
		//���Ի�ȡ����������ݣ�����Ĳ�����������ȡ����֡
		int GetNextData(int count, Data&pcmData);
		int GetNextFramesDataSizeInOriginalAudioData(int count);
		float GetAudioDuration();
		//audio data return the pcm format audio data
		//return done data byte count
		int SetAudioPosBySecond(float second);
		//����Feedģʽ�£�Ϊ��Ƶ�ڴ�������ԭʼ����Ƶ���ݣ����ں����Ľ���
		void Feed(unsigned char * mp3, int len);
	protected:
		//���øú���ǰ�����뱣֤ID3v2�Ѿ��������������������
		unsigned char * GetScanMP3FileDataStartPos();
		//���ص�ǰstart��ʼ��һ�������Ƿ��ǺϷ�����Ƶ֡
		bool TryToGetMP3Frame(unsigned char*start,int&framesize);
		//�½�һ����Ƶ֡�����Ѹ�֡��ӵ�������
		void NewMP3Frame(unsigned char * start,int framesize);
	public:
		static AudioResource*LoadAudioResource(Serializer::RuntimeAsset*name);
		static AudioResource*CreateAudioResourceWithRawData(const char*path,Data&data);
		DECLEAR_ALICE_CLASS(AudioResource)
	};
}
