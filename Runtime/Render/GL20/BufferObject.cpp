#include "BufferObject.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/Debugger/Log.h"
namespace Alice{
	BufferObject::BufferObject(GLenum type, int size, GLenum hint):mType(type){
		mIdentifier = 0;
		if (size != 0) {
			Init(type, size, hint);
		}
	}
	BufferObject::~BufferObject(){
        //Debug("BufferObject : delete %u",mIdentifier);
        if(mIdentifier!=0){
            OGL_CALL(glDeleteBuffers(1, &mIdentifier));
        }
	}
	void BufferObject::Init(GLenum type, int size, GLenum hint) {
		if (mIdentifier!=0){
			glDeleteBuffers(1, &mIdentifier);
		}
		mSize = size;
		OGL_CALL(glGenBuffers(1, &mIdentifier));
		OGL_CALL(glBindBuffer(mType, mIdentifier));
		OGL_CALL(glBufferData(mType, size, 0, hint));
		OGL_CALL(glBindBuffer(mType, 0));
	}
	void BufferObject::SubData(const void*data, int size, int offset) {
		if (mIdentifier != 0) {
			if (size > mSize) {
				Error("%u submit data to gpu fail,out of range %d max(%d)", mIdentifier,size,mSize);
				return;
			}
			OGL_CALL(glBindBuffer(mType, mIdentifier));
			OGL_CALL(glBufferSubData(mType, offset, size, data));
			OGL_CALL(glBindBuffer(mType, 0));
		}
	}
	bool BufferObject::Active(){
		if (mIdentifier == 0)return false;
		OGL_CALL(glBindBuffer(mType, mIdentifier));
		return true;
	}
	bool BufferObject::Restore() {
		if (mIdentifier == 0)return false;
		OGL_CALL(glBindBuffer(mType, 0));
		return true;
	}
}
