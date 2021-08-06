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
		//MP3Frame原始数据，指向MP3File的缓冲区中自己的首地址
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
		//初始化MP3文件的方式为：初始化ID3头部，然后再开始解码后面的音频帧。
		//目前东汉书院只支持ID3在文件头部的情况，对于ID3在文件末尾的情况不予理睬
		bool TryToInitHeader();
		//除了初始化MP3 ID3头部以外，还需要解码出前8帧的数据，以获得MP3的信息
		//也就是MPG123_NEW_FORMAT，拿到这一个返回。从此刻开始，就可以按照帧来解码数据。
		//单通道和多通道都可以这样做
		bool TryToGetMP3Info();
		void ScanMP3FileData();
		void SetCurrentFrame(int nFrameIndex);
		//尝试获取后面的流数据，传入的参数是期望获取多少帧
		int GetNextData(int count, Data&pcmData);
		int GetNextFramesDataSizeInOriginalAudioData(int count);
		float GetAudioDuration();
		//audio data return the pcm format audio data
		//return done data byte count
		int SetAudioPosBySecond(float second);
		//用于Feed模式下，为音频内存块中添加原始的音频数据，用于后续的解析
		void Feed(unsigned char * mp3, int len);
	protected:
		//调用该函数前，必须保证ID3v2已经被解码出来，否则会出错
		unsigned char * GetScanMP3FileDataStartPos();
		//返回当前start开始的一段数据是否是合法的音频帧
		bool TryToGetMP3Frame(unsigned char*start,int&framesize);
		//新建一个音频帧，并把该帧添加到链表里
		void NewMP3Frame(unsigned char * start,int framesize);
	public:
		static AudioResource*LoadAudioResource(Serializer::RuntimeAsset*name);
		static AudioResource*CreateAudioResourceWithRawData(const char*path,Data&data);
		DECLEAR_ALICE_CLASS(AudioResource)
	};
}
