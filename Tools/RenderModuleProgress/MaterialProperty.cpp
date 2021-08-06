#include "MaterialProperty.h"
MaterialProperty::MaterialProperty(MaterialUniformPropertyType t, const char * name) {
	mType = t;
	memset(mName, 0, 64);
	strcpy(mName, name);
}
MaterialPropertyMatrix4::MaterialPropertyMatrix4(const char * name):MaterialProperty(kMaterialUniformPropertyTypeMatrix4,name) {
	mPropertyValue = nullptr;
}
MaterialPropertyVec4::MaterialPropertyVec4(const char * name):MaterialProperty(kMaterialUniformPropertyTypeVec4,name) {
	mPropertyValue = nullptr;
}
MaterialProperty*MaterialPropertyMatrix4::Clone() {
	MaterialPropertyMatrix4*ret= new MaterialPropertyMatrix4(mName);
	ret->mPropertyValue = mPropertyValue;
	return ret;
}
MaterialProperty*MaterialPropertyVec4::Clone() {
	MaterialPropertyVec4*ret = new MaterialPropertyVec4(mName);
	ret->mPropertyValue = mPropertyValue;
	return ret;
}
MaterialPropertySampler2D::MaterialPropertySampler2D(const char* name) :MaterialProperty(kMaterialUniformPropertyTypeSampler2D, name) {
	mTextureName = 0;
}
MaterialProperty* MaterialPropertySampler2D::Clone() {
	MaterialPropertySampler2D* ret = new MaterialPropertySampler2D(mName);
	ret->mTextureName = mTextureName;
	return ret;
}