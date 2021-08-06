#include "UniformUpdater.h"
#include "MaterialProperty.h"
#include "Runtime/Render/RenderState.h"
#include "Runtime/Debugger/Debugger.h"
#include "Camera.h"
#include "Runtime/2D/Canvas.h"
namespace Alice{
	UniformUpdater::UniformUpdater(SetUniformFoo foo,GLint loc,void*userData):mLocation(loc),mFoo(foo),mUserData(userData){
	}
	UniformUpdater::UniformUpdater(UniformUpdater*uniform) : mLocation(uniform->mLocation), mFoo(uniform->mFoo),mUserData(uniform->mUserData){
	}
	UniformUpdater::~UniformUpdater(){
	}
	void UniformUpdater::Update(void* camera){
		if(mFoo!=nullptr)mFoo(mLocation, camera,mUserData);
		if (mNext!=nullptr)
		{
			Next<UniformUpdater>()->Update(camera);
		}
	}
	void UniformUpdater::operator=(UniformUpdater&r){
		mFoo = r.mFoo;
		mLocation = r.mLocation;
		mUserData = r.mUserData;
	}
	void UniformUpdater::UpdateTextureUnit(GLint loc, void*camera, void*userData) {
		if (userData != nullptr) {
			TextureUnitProperty*prop = (TextureUnitProperty*)userData;
			if (prop->mValue != nullptr) {
				OGL_CALL(glActiveTexture(GL_TEXTURE0+prop->mSlot));
				OGL_CALL(glBindTexture(prop->mValue->mTextureType, prop->mValue->mTextureID));
				OGL_CALL(glUniform1i(loc, prop->mSlot));
			}
		}
	}
	void UniformUpdater::UpdateProjectionMatrix(GLint loc, void*camera, void*userData) {
		Camera*c = (Camera*)camera;
		OGL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, c->mProjectionMatrix.mData));
	}
	void UniformUpdater::UpdateViewMatrix(GLint loc, void*camera, void*userData) {
		Camera*c = (Camera*)camera;
		OGL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, c->mViewMatrix.mData));
	}
	void UniformUpdater::UpdateCanvas(GLint loc, void*canvas, void*userData) {
		Canvas *c = (Canvas*)canvas;
		OGL_CALL(glUniform4fv(loc, 1, c->mCanvasInfo.v)); 
	}
	void UniformUpdater::UpdateFloat(GLint loc, void*camera, void*userData) {
		UniformFloatProperty*float_property = (UniformFloatProperty*)userData;
		OGL_CALL(glUniform1fv(loc,1,&float_property->mValue));
	}
	void UniformUpdater::UpdateVec2(GLint loc, void*camera, void*userData) {
		UniformVector2Property*vec2 = (UniformVector2Property*)userData;
		OGL_CALL(glUniform2fv(loc, 1, vec2->mValue.v));
	}
	void UniformUpdater::UpdateVec3(GLint loc, void*camera, void*userData) {
		UniformVector3Property*vec3 = (UniformVector3Property*)userData;
		OGL_CALL(glUniform3fv(loc, 1, vec3->mValue.v));
	}
	void UniformUpdater::UpdateVec4(GLint loc, void*camera, void*userData){
		UniformVector4Property*vec4 = (UniformVector4Property*)userData;
		OGL_CALL(glUniform4fv(loc, 1, vec4->mValue.v));
	}
	void UniformUpdater::UpdateIntArray(GLint loc, void*camera, void*userData) {
		UniformIntArrayProperty*vec4 = (UniformIntArrayProperty*)userData;
		if (vec4->mSize == 0) {
			return;
		}
		OGL_CALL(glUniform1iv(loc, vec4->mSize, vec4->mValue));
	}
	void UniformUpdater::UpdateVec4Array(GLint loc, void*camera, void*userData){
		UniformVector4ArrayProperty*vec4 = (UniformVector4ArrayProperty*)userData;
		if (vec4->mSize == 0) {
			return;
		}
		OGL_CALL(glUniform4fv(loc, vec4->mSize, vec4->mValue));
	}
	void UniformUpdater::UpdateMat3(GLint loc, void*camera, void*userData) {
		UniformMatrix3Property*mat3 = (UniformMatrix3Property*)userData;
		OGL_CALL(glUniformMatrix3fv(loc, 1, GL_FALSE, mat3->mValue.mData));
	}
	void UniformUpdater::UpdateMat4(GLint loc, void*camera, void*userData) {
		UniformMatrix4Property*mat4 = (UniformMatrix4Property*)userData;
		OGL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, mat4->mValue.mData));
	}
}