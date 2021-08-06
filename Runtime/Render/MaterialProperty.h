#pragma once
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/Math/Vector2.h"
#include "Runtime/Math/Vector4.h"
#include "Runtime/Math/AliceMatrix4x4.h"
#include "Runtime/Math/Matrix3x3.h"
#include "Runtime/Utils/SmartPtr.h"
#include "GL20/Texture2D.h"
#include "GL20/TextureCube.h"
namespace Alice{
	enum UniformType {
		kUniformTypeMatrix3 = 1,
		kUniformTypeMatrix4,
		kUniformTypeSamplerCube,
		kUniformTypeSampler2D,
		kUniformTypeFloat,
		kUniformTypeIntArray,
		kUniformTypeVec2,
		kUniformTypeVec3,
		kUniformTypeVec4,
		kUniformTypeVector4Array,
		kUniformTypeCount
	};
	class MaterialProperty{
	public:
		MaterialProperty(UniformType type):mValueType(type),mLocation(-1),mName(64){
			mLocation = -1;
		}
		virtual ~MaterialProperty(){
		}
		virtual MaterialProperty* Clone() = 0;
		UniformType mValueType;
		GLint	mLocation;
		FixedString mName;
	};
	class TextureUnitProperty :public MaterialProperty {
	public:
		TextureUnitProperty(UniformType type) :
			MaterialProperty(type),
			mSlot(-1),
			mValue(nullptr){
		}
		virtual MaterialProperty* Clone();
		GLint mSlot;
		SmartPtr<TextureUnit> mValue;
	};
	class UniformFloatProperty :public MaterialProperty{
	public:
		UniformFloatProperty(UniformType type = kUniformTypeFloat) :MaterialProperty(type), mValue(0.0f) {
		}
		virtual MaterialProperty* Clone();
		float mValue;
	};
	class UniformVector2Property :public MaterialProperty{
	public:
		UniformVector2Property(UniformType type = kUniformTypeVec2) :MaterialProperty(type), mValue(0.0f, 0.0f) {
		}
		virtual MaterialProperty* Clone();
		Vector2f mValue;
	};
	class UniformVector3Property :public MaterialProperty{
	public:
		UniformVector3Property(UniformType type = kUniformTypeVec3) :MaterialProperty(type), mValue(0.0f, 0.0f, 0.0f) {
		}
		virtual MaterialProperty* Clone();
		Vector3f mValue;
	};
	class UniformVector4Property :public MaterialProperty{
	public:
		UniformVector4Property(UniformType type = kUniformTypeVec4):MaterialProperty(type),mValue(0.0f,0.0f,0.0f,0.0f){
		}
		virtual MaterialProperty* Clone();
		Vector4f mValue;
	};
	class UniformVector4ArrayProperty :public MaterialProperty{
	public:
		UniformVector4ArrayProperty(UniformType type = kUniformTypeVector4Array):
			MaterialProperty(type),
			mSize(0),
			mValue(nullptr){
		}
		virtual MaterialProperty*Clone();
		int mSize;
		float *mValue;
	};
	class UniformIntArrayProperty :public MaterialProperty{
	public:
		UniformIntArrayProperty(UniformType type = kUniformTypeIntArray):
			MaterialProperty(type),
			mSize(0),
			mValue(nullptr){
		}
		int mSize;
		int *mValue;
		virtual MaterialProperty*Clone();
	};
	class UniformMatrix4Property : public MaterialProperty{
	public:
		UniformMatrix4Property(UniformType type = kUniformTypeMatrix4):
			MaterialProperty(type){
		}
		virtual MaterialProperty*Clone();
		Matrix4x4 mValue;
	};
	class UniformMatrix3Property : public MaterialProperty{
	public:
		UniformMatrix3Property(UniformType type = kUniformTypeMatrix3):
			MaterialProperty(type){
		}
		virtual MaterialProperty*Clone();
		Matrix3x3 mValue;
	};
	struct AttributeDefine{
		AliceSInt32 mAttributeSlot;
		AliceSInt32 mDataOffSet;
		GLenum mDataType;
		AliceUInt8 mUnitCount;
		GLboolean mNormalized;
		AliceUInt16 mDataStride;
		AttributeDefine() :mAttributeSlot(-1), mDataOffSet(0), mNormalized(GL_FALSE){
		}
	};

}