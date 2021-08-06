#pragma once
#include "Runtime/Base/Object.h"
namespace Alice {
	class BinaryBuffer : public Object{
	public:
		BinaryBuffer();
		~BinaryBuffer();
		AliceUInt8 * mBuffer;
		int mDataSize;
		int mSize;
		void SetSize(int size);
		void AppendInt(int value);
		void Append(const char*data,int count);
		void Shift(int count);
		void TrimEnd(int count);
		void Reset();
		static AliceSInt32 GetNextInt(const char*buffer);
	private:
		BinaryBuffer(const BinaryBuffer&r) {}
		BinaryBuffer&operator=(const BinaryBuffer&r) { return *this; };
	public:
		DECLEAR_ALICE_CLASS(BinaryBuffer)
	};
}