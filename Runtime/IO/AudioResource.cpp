#include "AudioResource.h"
#include "Runtime/Math/AliceFloat.h"
#include "Runtime/Serializer/Audio.serializer.h"
#ifdef ALICE_PLATFORM_WIN
#pragma comment(lib,"libmpg123.lib")
#endif
namespace Alice
{
	struct mpg123Initiallizer
	{
	public:
		mpg123Initiallizer()
		{
			mpg123_init();
		}
	};
	static mpg123Initiallizer mpg123Init;
	AudioResource::AudioResource() {
		mFrameCount = 0;
		mChannelCount = 0;
		mbCached = false;
		mRawData = nullptr;
		mTPF = 0.026122f;
		mHandle = nullptr;
		mCurrentFrame = nullptr;
		mHeadFrame = nullptr;
		mEndFrame = nullptr;
		mID3Tag = nullptr;
		mCurrentFrameIndex = -1;//已经到了文件末尾
    }
    AudioResource::~AudioResource(){
		if (mResourceData==nullptr&&mRawData!=nullptr){
			delete[] mRawData;//the data is alloced by new
		}
		if (mHandle!=nullptr){
			mpg123_close(mHandle);
			mpg123_delete(mHandle);
		}
		if (mHeadFrame!=nullptr){
			if (mHeadFrame != nullptr) {
				MP3Frame*current = mHeadFrame;
				while (current) {
					MP3Frame*next = current->Next<MP3Frame>();
					current->mNext = nullptr;
					delete current;
					current = next;
				}
			}
			mHeadFrame = nullptr;
			mCurrentFrame = nullptr;
			mEndFrame = nullptr;
		}
		if (mID3Tag != nullptr) {
			delete mID3Tag;
		}
	}
	bool AudioResource::TryToInitHeader() {
		if (mID3Tag != nullptr) {
			return true;
		}
		mID3Tag = load_tag_with_buffer((char*)mRawData, mCachedRawDataSize);
		if (mID3Tag != nullptr) {
			return true;
		}
		return false;
	}
	bool AudioResource::TryToGetMP3Info() {
		if (mChannelCount != 0) {//如果ChannelCount已经有数据了，就表示MP3的基本信息已经拿到了
			return true;
		}
		if (mEndFrame==nullptr){
			return false;
		}
		if (mEndFrame->mFrameIndex<7) {//如果已经缓冲了8帧音频，那么就开始解码并获取音频文件的相关参数，8帧约1.5KB数据
			return false;
		}
		//如果8帧已经缓冲完毕，那么就可以开始解码MP3信息了
		MP3Frame*frame = mCurrentFrame;
		int len = GetNextFramesDataSizeInOriginalAudioData(8);
		if (mHandle!=nullptr){
			mpg123_close(mHandle);
			mpg123_delete(mHandle);
		}
		mHandle = mpg123_new(nullptr, nullptr);
		mpg123_open_feed(mHandle);
		int nRet = mpg123_feed(mHandle, frame->mMP3FrameData, len);
		if (nRet==MPG123_OK){
			long rate = 0;
			mpg123_getformat(mHandle, &rate, &mChannelCount, &mEncoding);
			mFrequence = rate;
			if (mEncoding == MPG123_ENC_UNSIGNED_8) {
				mFormat = mChannelCount == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
			}
			else {//unsigned 16
				mFormat = mChannelCount == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
			}
		}
		mpg123_close(mHandle);
		mpg123_delete(mHandle);
		mHandle = nullptr;
		mCurrentFrame = mHeadFrame;
		mCurrentFrameIndex = 0;
		return mChannelCount!=0;
	}
	int AudioResource::GetNextFramesDataSizeInOriginalAudioData(int count) {
		int ret = 0;
		//int i = 0;
		for (int i = 0; i < count; ++i) {
			ret += mCurrentFrame->mFrameSizeInByte;
			mCurrentFrame = mCurrentFrame->Next<MP3Frame>();
			++mCurrentFrameIndex;
		}
		return ret;
	}
	int findFrameSamplingFrequency(const unsigned char ucHeaderByte){
		unsigned int freq[] = { 44100,48000,32000,00000 };
		unsigned char ucTest = ucHeaderByte & 0x0C;
		ucTest = ucTest >> 6;
		int unFrameSamplingFrequency = freq[(unsigned int)ucTest];
		return unFrameSamplingFrequency;
	}
	int findFrameBitRate(const unsigned char ucHeaderByte){
		unsigned int bitrate[] = { 0,32000,40000,48000,56000,64000,80000,96000,
			112000,128000,160000,192000,224000,256000,320000,0 };
		unsigned char ucTest = ucHeaderByte & 0xF0;
		ucTest = ucTest >> 4;
		int unFrameBitRate = bitrate[(unsigned int)ucTest];
		return unFrameBitRate;
	}
	int findMpegVersionAndLayer(const unsigned char ucHeaderByte){
		int MpegVersionAndLayer;
		unsigned char ucTest = ucHeaderByte & 0x1E;
		if (ucTest == 0x1A){
			MpegVersionAndLayer = 1;
		}else{
			MpegVersionAndLayer = 1;
		}
		return MpegVersionAndLayer;
	}

	int findFramePadding(const unsigned char ucHeaderByte){
		unsigned char ucTest = ucHeaderByte & 0x02;
		int nFramePadded;
		if ((unsigned int)ucTest == 2) {
			nFramePadded = 1;
		}else {
			nFramePadded = 0;
		}
		return nFramePadded;
	}
	void AudioResource::ScanMP3FileData() {
		if (mID3Tag==nullptr){
			return;
		}
		unsigned char *endPos = mRawData + mCachedRawDataSize;
		unsigned char *startPos = GetScanMP3FileDataStartPos();
		//Debug("ScanMP3FileData from %d to %d(%d)",startPos-mRawData,endPos-mRawData,mCachedRawDataSize);
		while (startPos < endPos) {
			int nFrameLen = 0;
			if (startPos+4>endPos){//在尝试获取一帧之前，必须保证还有四个字节是可用的
				break;
			}
			if (TryToGetMP3Frame(startPos,nFrameLen)){
				if ((startPos+nFrameLen)>endPos){
					break;
				}
				NewMP3Frame(startPos, nFrameLen);
				startPos += nFrameLen;
			} else {
				++startPos;
			}
		}
		if (mbCached) {//全部解码完毕
			mFrameCount = mEndFrame->mFrameIndex+1;
		}
	}
	unsigned char * AudioResource::GetScanMP3FileDataStartPos() {
		return mEndFrame == nullptr ? mRawData + mID3Tag->tag_header->tag_size + 10 : mEndFrame->mMP3FrameData + mEndFrame->mFrameSizeInByte;
	}
	bool AudioResource::TryToGetMP3Frame(unsigned char*start,int&framesize) {
		unsigned char ucHeaderByte1, ucHeaderByte2, ucHeaderByte3, ucHeaderByte4;
		ucHeaderByte1 = *start++;
		if (ucHeaderByte1 == 0xFF) {
			ucHeaderByte2 = *start++;
			unsigned char ucByte2LowerNibble = ucHeaderByte2 & 0xF0;
			if (ucByte2LowerNibble == 0xF0 || ucByte2LowerNibble == 0xE0) {
				ucHeaderByte3 = *start++;
				ucHeaderByte4 = *start++;
				int nFrameSamplingFrequency = findFrameSamplingFrequency(ucHeaderByte3);
				int nFrameBitRate = findFrameBitRate(ucHeaderByte3);
				int nMpegVersionAndLayer = findMpegVersionAndLayer(ucHeaderByte2);
				if (nFrameBitRate == 0 || nFrameSamplingFrequency == 0 || nMpegVersionAndLayer == 0) {
					//bad mp3 data if this happens then we must have found the sync word but it was not actually part of the header
					Error("bad mp3 data");
					return false;
				}
				int nFramePadded = findFramePadding(ucHeaderByte3);
				//frame length包含了头部的4字节
				framesize = int((144 * (float)nFrameBitRate /(float)nFrameSamplingFrequency) + nFramePadded);
				return true;
			}
		}
		return false;
	}
	void AudioResource::NewMP3Frame(unsigned char * start, int framesize) {
		MP3Frame*frameInfo = new MP3Frame;
		frameInfo->mFrameIndex = mEndFrame==nullptr?0:mEndFrame->mFrameIndex+1;
		frameInfo->mFrameSizeInByte = framesize;
		frameInfo->mMP3FrameData = start;
		if (mHeadFrame == nullptr) {
			mHeadFrame = frameInfo;
			mEndFrame = frameInfo;
			mCurrentFrame = frameInfo;
			mCurrentFrameIndex = frameInfo->mFrameIndex;
		}
		else {
			if (mCurrentFrame == nullptr) {
				mCurrentFrame = frameInfo;
				mCurrentFrameIndex = frameInfo->mFrameIndex;
			}
			mEndFrame->PushBack(frameInfo);
			mEndFrame = frameInfo;
		}
	}
	void AudioResource::SetCurrentFrame(int nFrameIndex) {
		if (mHeadFrame==nullptr){
			return;
		}
		MP3Frame*current = mHeadFrame;
		while (current != nullptr&&current->mFrameIndex != nFrameIndex) {
			current = current->Next<MP3Frame>();
		}
		if (current != nullptr) {
			mCurrentFrame = current;
			mCurrentFrameIndex = current->mFrameIndex;
		}
		else {
			mCurrentFrame = nullptr;
			mCurrentFrameIndex = mEndFrame->mFrameIndex;
		}
	}
	int AudioResource::GetNextData(int count, Data&pcmData) {
		if (mID3Tag == nullptr) {
			if (TryToInitHeader() == false) {
				return -1;//not cached,wait for data,cannot decode id3v2 header tag
			}
		}
		if (mChannelCount == 0) {
			if (TryToGetMP3Info() == false) {
				return -1;//not cached,wait for data,cannot decode mp3 info
			}
		}
		int fixed_frame_count = count;
		if (mCurrentFrameIndex+count<mEndFrame->mFrameIndex){//已经缓冲并解码了该音频帧
		} else {//还没有解码出该音频帧
			if (mFrameCount == 0) {//没有缓冲完毕
				return -1;
			}
			else {//已经缓冲完毕了，但是任然不足，则计算最后还剩下多少帧可以解码
				fixed_frame_count = mFrameCount - mCurrentFrameIndex - 2;
			}
		}
		if (fixed_frame_count<=0){
			return -2;
		}
		unsigned char*start = mCurrentFrame->mMP3FrameData;
		int dataSize = GetNextFramesDataSizeInOriginalAudioData(fixed_frame_count);
		//Debug("decode frame count %d %d %d", fixed_frame_count, start - mRawData, dataSize);
		if (dataSize == 0) {//这里表示已经到了文件结尾了
			pcmData.mDataLen = 0;
			return -2;
		}
		//开始对每一帧解码，将原始MP3帧数据解码成为PCM数据
		size_t nDecode = 0;
		if (mHandle==nullptr){
			mHandle = mpg123_new(nullptr, nullptr);
			mpg123_open_feed(mHandle);
		}
		int nRet = mpg123_decode(mHandle, start, dataSize, pcmData.mData, pcmData.mBufferSize, &nDecode);
		//Debug("decode %d %d",pcmData.mBufferSize,nDecode);
		if (nRet == MPG123_NEW_FORMAT) {
			nRet = mpg123_read(mHandle, pcmData.mData, pcmData.mBufferSize, &nDecode);
			pcmData.mDataLen = nDecode;
		}else {
			pcmData.mDataLen = nDecode;
		}
		return 0;
	}
	int AudioResource::SetAudioPosBySecond(float second) {
		if (mHeadFrame==nullptr){
			return -1;
		}
		int nIndex = FloorfToInt(second / mTPF);
		nIndex /= 8;
		SetCurrentFrame(nIndex*8);
		return mCurrentFrame->mFrameIndex;
	}

	void AudioResource::Feed(unsigned char * mp3, int len) {
		if (mbCached){
			return;
		}
		if (mCachedRawDataSize+len>mRawDataBufferSize){
			Error("too much data");
			return;
		}
		memcpy(mRawData + mCachedRawDataSize, mp3, len);
		mCachedRawDataSize += len;
		if (mCachedRawDataSize == mRawDataBufferSize) {
			mbCached = true;
		}
		if (mID3Tag == nullptr) {
			if (TryToInitHeader() == false) {
				return;//not cached,wait for data,cannot decode id3v2 header tag
			}
		}
        ScanMP3FileData();
		if (mChannelCount == 0) {
			if (TryToGetMP3Info() == false) {
				return;//not cached,wait for data,cannot decode mp3 info
			}
		}
	}
	float AudioResource::GetAudioDuration() {
		return mFrameCount * mTPF;
	}
	AudioResource*AudioResource::LoadAudioResource(Serializer::RuntimeAsset*asset){
		Serializer::RuntimeAudioData*rad = new (kMemAudioId)Serializer::RuntimeAudioData;
		if (rad->ParseFromArray(asset->rawdata().c_str(), asset->rawdata().length())){
			AudioResource*audioResource = new (kMemAudioId)AudioResource;
			audioResource->mResourceData = rad;
			audioResource->mName = asset->guid();
			audioResource->mRawData = (unsigned char*)rad->data().c_str();
			audioResource->mCachedRawDataSize = rad->data().size();
			audioResource->mRawDataBufferSize = audioResource->mCachedRawDataSize;
			audioResource->mbCached = true;
			audioResource->TryToInitHeader();
            audioResource->ScanMP3FileData();
			audioResource->TryToGetMP3Info();
			return audioResource;
		}else{
			delete rad;
		}
		return nullptr;
	}
	AudioResource*AudioResource::CreateAudioResourceWithRawData(const char*path, Data&data){
		Serializer::RuntimeAudioData*rad = new (kMemAudioId)Serializer::RuntimeAudioData;
		rad->set_data(data.mData, data.mDataLen);
		rad->set_format(1);
		AudioResource*audioResource = new (kMemAudioId)AudioResource;
		audioResource->mResourceData = rad;
		audioResource->mName = path;
		audioResource->mRawData = (unsigned char*)rad->data().c_str();
		audioResource->mCachedRawDataSize = rad->data().size();
		audioResource->mRawDataBufferSize = audioResource->mCachedRawDataSize;
		audioResource->mbCached = true;
		audioResource->TryToInitHeader();
        audioResource->ScanMP3FileData();
		audioResource->TryToGetMP3Info();
		return audioResource;
	}
}
