#pragma once
#include "AliceGL.h"
#include "LinkedList.h"
enum MaterialUniformPropertyType {
	kMaterialUniformPropertyTypeCameraProjectionMatrix,
	kMaterialUniformPropertyTypeCameraViewMatrix,
	kMaterialUniformPropertyTypeMatrix4,
	kMaterialUniformPropertyTypeVec4,
	kMaterialUniformPropertyTypeSampler2D,
	kMaterialUniformPropertyTypeCount
};
class MaterialProperty {
public:
	MaterialUniformPropertyType mType;//uniform float U_Factor;
	char mName[64];
	MaterialProperty(MaterialUniformPropertyType t,const char * name);
	virtual MaterialProperty*Clone() = 0;
};
class MaterialPropertyMatrix4 :public MaterialProperty {
public:
	MaterialPropertyMatrix4(const char * name);
	float*mPropertyValue;
	MaterialProperty*Clone();
};
class MaterialPropertyVec4 :public MaterialProperty {
public:
	MaterialPropertyVec4(const char * name);
	float*mPropertyValue;
	MaterialProperty*Clone();
};
class MaterialPropertySampler2D :public MaterialProperty {
public:
	MaterialPropertySampler2D(const char* name);
	GLuint mTextureName;//TextureUnit
	MaterialProperty* Clone();
};