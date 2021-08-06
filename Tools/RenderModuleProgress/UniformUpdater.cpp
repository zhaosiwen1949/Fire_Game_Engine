#include "UniformUpdater.h"

UniformUpdater::UniformUpdater(GLint location, UpdateUniformFunction f, MaterialProperty*target_property) {
	mLocation = location;
	mUpdateFunction = f;
	mTargetProperty = target_property;
}
void UniformUpdater::UpdateProjectionMatrix4(GLint location, void*camera, void*property_value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(((Camera*)camera)->mProjectionMatrix));
}
void UniformUpdater::UpdateViewMatrix4(GLint location, void*camera, void*property_value) {
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(((Camera*)camera)->mViewMatrix));
}
void UniformUpdater::UpdateMatrix4(GLint location, void*camera, void*property_value) {
	MaterialPropertyMatrix4*prop = (MaterialPropertyMatrix4*)property_value;
	glUniformMatrix4fv(location, 1, GL_FALSE, prop->mPropertyValue);
}
void UniformUpdater::UpdateVec4(GLint location, void*camera, void*property_value) {
	MaterialPropertyVec4*prop = (MaterialPropertyVec4*)property_value;
	glUniform4fv(location, 1, prop->mPropertyValue);
}
void UniformUpdater::UpdateSampler2D(GLint location, void* camera, void* property_value) {
	MaterialPropertySampler2D* prop = (MaterialPropertySampler2D*)property_value;
	glUniform1i(location,prop->mTextureName);
}
UniformUpdater*UniformUpdater::Clone() {
	return new UniformUpdater(mLocation,mUpdateFunction,mTargetProperty==nullptr?mTargetProperty : mTargetProperty->Clone());
}