#include "BinaryBuffer.h"
namespace Alice {
	BinaryBuffer::BinaryBuffer() {
		mBuffer = nullptr;
		mDataSize = 0;
		mSize = 0;
	}
	BinaryBuffer::~BinaryBuffer() {
		if (mSize!=0){
			delete [] mBuffer;
		}
	}
	void BinaryBuffer::SetSize(int size) {
		mSize = size;
		mBuffer = new AliceUInt8[mSize];
		memset(mBuffer, 0, mSize);
	}
	void BinaryBuffer::AppendInt(int value){
		memcpy(mBuffer + mDataSize, &value, sizeof(int));
		mDataSize += sizeof(int);
	}
	void BinaryBuffer::Append(const char*data,int count) {
		memcpy(mBuffer + mDataSize, data, count);
		mDataSize += count;
	}
	void BinaryBuffer::Shift(int count) {
		if (count==0){
			return;
		}
		if (count>=mDataSize){
			Reset();
		}else{
			int nDataLeft = mDataSize - count;
			memcpy(mBuffer, mBuffer + count, nDataLeft);
			memset(mBuffer + nDataLeft, 0, count);
			mDataSize -= count;
		}
	}
	void BinaryBuffer::TrimEnd(int count) {

	}
	void BinaryBuffer::Reset() {
		if (mSize > 0) {
			memset(mBuffer, 0, mSize);
			mDataSize = 0;
		}
	}
	AliceSInt32 BinaryBuffer::GetNextInt(const char*buffer) {
		AliceSInt32 value = *((AliceSInt32*)buffer);
		if (value<0){
			return -1;
		}
		return value;
	}
}