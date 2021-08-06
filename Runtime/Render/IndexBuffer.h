#pragma once
#include "Runtime/RuntimePrefix.h"
namespace Alice{
	class IndexBuffer
	{
	public:
		IndexBuffer(AliceUInt16 size=0);
        ~IndexBuffer();
		AliceSInt32 mSize;
		AliceSInt32 mBufferSize;
		AliceUInt16 *mIndexes;
		void Init(AliceUInt16 size);
	};
}
