#pragma once
#include "AliceGL.h"
#include "LinkedList.h"
#include "MaterialProperty.h"
struct Camera{
	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewMatrix;
};
typedef void(*UpdateUniformFunction)(GLint location,void*camera,void*property_value);
class UniformUpdater : public LinkedList {
public:
	GLint mLocation;
	UpdateUniformFunction mUpdateFunction; 
	MaterialProperty*mTargetProperty;
	UniformUpdater(GLint location, UpdateUniformFunction f, MaterialProperty*target_property);
	UniformUpdater * Clone();
	static void UpdateProjectionMatrix4(GLint location, void*camera, void*property_value);
	static void UpdateViewMatrix4(GLint location, void*camera, void*property_value);
	static void UpdateMatrix4(GLint location, void*camera, void*property_value);
	static void UpdateVec4(GLint location, void*camera, void*property_value);
	static void UpdateSampler2D(GLint location, void* camera, void* property_value);
};