#pragma once
#include "Runtime/RuntimePrefix.h"
namespace Alice{
	class Data{
	public:
		Data(int buffer_size=0);
		virtual ~Data();
		void Reset();
		void SetBufferSize(int bufferSize);
		void MakeZero();
		AliceUInt8 *mData;
		AliceSInt32 mDataLen;
		AliceSInt32 mBufferSize;
	};
}