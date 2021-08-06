#include "MaterialProperty.h"
#include "Runtime/Allocator/DefaultAllocator.h"

namespace Alice
{
	MaterialProperty* TextureUnitProperty::Clone()
	{
		TextureUnitProperty*ret = new (kMemMaterialId)TextureUnitProperty(mValueType);
		ret->mName = mName.mText;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		ret->mSlot = mSlot;
		return ret;
	}

	MaterialProperty* UniformFloatProperty::Clone()
	{
		UniformFloatProperty*ret = new (kMemMaterialId)UniformFloatProperty;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		return ret;
	}
	MaterialProperty* UniformVector2Property::Clone()
	{
		UniformVector2Property*ret = new (kMemMaterialId)UniformVector2Property;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		return ret;
	}
	MaterialProperty* UniformVector3Property::Clone()
	{
		UniformVector3Property*ret = new (kMemMaterialId)UniformVector3Property;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		return ret;
	}
	MaterialProperty* UniformVector4Property::Clone()
	{
		UniformVector4Property*ret = new (kMemMaterialId)UniformVector4Property;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		return ret;
	}

	MaterialProperty*UniformVector4ArrayProperty::Clone()
	{
		UniformVector4ArrayProperty*ret = new (kMemMaterialId)UniformVector4ArrayProperty;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		ret->mSize = mSize;
		return ret;
	}

	MaterialProperty*UniformIntArrayProperty::Clone()
	{
		UniformIntArrayProperty*ret = new (kMemMaterialId)UniformIntArrayProperty;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		ret->mSize = mSize;
		return ret;
	}

	MaterialProperty*UniformMatrix4Property::Clone()
	{
		UniformMatrix4Property*ret = new (kMemMaterialId)UniformMatrix4Property;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		ret->mValue = mValue;
		return ret;
	}

	MaterialProperty*UniformMatrix3Property::Clone()
	{
		UniformMatrix3Property*ret = new (kMemMaterialId)UniformMatrix3Property;
		ret->mLocation = mLocation;
		ret->mValueType = mValueType;
		memcpy(ret->mValue.mData, mValue.mData, sizeof(float) * 16);
		return ret;
	}
}