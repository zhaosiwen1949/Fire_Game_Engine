#include "Attribute.h"

Attribute::Attribute(GLint location, int componentCount, unsigned int basicDataType, bool normalize, int dataStride, int dataOffset) {
	mLocation = location;
	mComponentCount = componentCount;
	mBasicDataType = basicDataType;
	mbNormalize = normalize;
	mDataStride = dataStride;
	mDataOffset = dataOffset;
}
void Attribute::Active() {
	glEnableVertexAttribArray(mLocation);
	glVertexAttribPointer(mLocation, mComponentCount, mBasicDataType, mbNormalize, mDataStride, (void*)mDataOffset);
	if (mNext!=nullptr){
		Next<Attribute>()->Active();
	}
}