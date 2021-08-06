#include "IndexBuffer.h"
namespace Alice{
	IndexBuffer::IndexBuffer(AliceUInt16 size):mIndexes(nullptr){
		mSize = size;
		mBufferSize = 0;
		if (mSize != 0) {
			mIndexes = new AliceUInt16[mSize];
			mBufferSize = sizeof(AliceUInt16)*mSize;
			memset(mIndexes, 0, mBufferSize);
		}
	}
    IndexBuffer::~IndexBuffer(){
        if (mIndexes!=nullptr){
            delete mIndexes;
			mIndexes = nullptr;
        }
		mSize = 0;
		mBufferSize = 0;
    }
	void IndexBuffer::Init(AliceUInt16 size) {
		mSize = size;
		mIndexes = new AliceUInt16[mSize];
		mBufferSize = sizeof(AliceUInt16)*mSize;
		memset(mIndexes, 0, mBufferSize);
	}
}
