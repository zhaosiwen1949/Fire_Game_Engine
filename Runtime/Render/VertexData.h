#pragma once
#include "Runtime/Math/Vector2.h"
#include "Runtime/Math/Vector3.h"
#include "Runtime/Math/Vector4.h"
#include "Runtime/Math/AliceMatrix4x4.h"
#include "Runtime/Math/Color.h"
namespace Alice{
	enum VertexDataType{
		kVertexDataTypeFull,
		kVertexDataTypeES20Full,
		kVertexDataTypeES30Full,
		kVertexDataTypeCount
	};
	//80 bytes
	struct VertexDataFull{
		Vector4f mVertex;//16bytes
		Vector4f mTexCoord0;//16 bytes
		Vector4f mNormal;//16bytes
		Vector4f mTangent;//16 bytes
		Vector4f mTexCoord1;//16 bytes
	};
	//opengl es 2.0 max attribute count 8 : 128 bytes
	struct VertexDataES2_0_Full {
		Vector4f mVertex;//16bytes
		Vector4f mTexCoord0;//16 bytes
		Vector4f mNormal;//16bytes
		Vector4f mTangent;//16 bytes
		Vector4f mTexCoord1;//16 bytes
		Vector4f mTexCoord2;//16 bytes
		Vector4f mTexCoord3;//16 bytes
		Vector4f mTexCoord4;//16 bytes
	};
	//opengl es 3.0 max attribute count 16  : 256 bytes
	struct VertexDataES3_0_Full {
		Vector4f mVertex;//16bytes
		Vector4f mTexCoord0;//16 bytes
		Vector4f mNormal;//16bytes
		Vector4f mTangent;//16 bytes
		Vector4f mTexCoord1;//16 bytes
		Vector4f mTexCoord2;//16 bytes
		Vector4f mTexCoord3;//16 bytes
		Vector4f mTexCoord4;//16 bytes
		Vector4f mTexCoord5;//16 bytes
		Vector4f mTexCoord6;//16 bytes
		Vector4f mTexCoord7;//16 bytes
		Vector4f mTexCoord8;//16 bytes
		Vector4f mTexCoord9;//16 bytes
		Vector4f mTexCoord10;//16 bytes
		Vector4f mTexCoord11;//16 bytes
		Vector4f mTexCoord12;//16 bytes
	};
	class VertexData{
	public:
		VertexData();
        ~VertexData();
		AliceSInt32 mType;
		AliceSInt32 mCount;
		AliceSInt32 mBufferSize;
		AliceAny mData;
		void SetBufferSize(int count);
		template<typename T>
		T* GetBuffer() {
			return (T*)mData;
		}
	};
}
