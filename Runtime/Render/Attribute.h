#pragma once
#include "Runtime/Utils/LinkedList.h"
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/AliceGL.h"

namespace Alice{
	class Attribute :public LinkedList{
	public:
		Attribute();
		~Attribute();
		void Enable();
		AliceSInt32 mAttributeSlot;
		AliceSInt32 mDataOffSet;
		GLenum mDataType;
		AliceUInt8 mUnitCount;
		GLboolean mNormalized;
		AliceUInt16 mDataStride;
	};
}