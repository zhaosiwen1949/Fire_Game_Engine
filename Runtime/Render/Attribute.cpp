#include "Attribute.h"
#include "Runtime/Debugger/Debugger.h"
#include "Runtime/Render/RenderState.h"
namespace Alice{
	Attribute::Attribute():mAttributeSlot(-1), mDataOffSet(0), mNormalized(GL_FALSE){

	}
	Attribute::~Attribute(){
		if (mNext!=nullptr){
			delete Next<Attribute>();
		}
	}
	void Attribute::Enable(){
		GlobalRenderState::EnableAttribute(mAttributeSlot,true);
		OGL_CALL(glVertexAttribPointer(mAttributeSlot, mUnitCount,mDataType, mNormalized, mDataStride, (void*)mDataOffSet));
		if (mNext!=nullptr){
			Next<Attribute>()->Enable(); 
		}
	}
}