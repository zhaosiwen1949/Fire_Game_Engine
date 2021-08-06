#pragma once
#include "AliceGL.h"
#include "LinkedList.h"
class Attribute : public LinkedList {
public:
	GLint mLocation;
	int mComponentCount;
	unsigned int mBasicDataType;
	bool mbNormalize;
	int mDataStride;
	int mDataOffset;
	Attribute(GLint location,
		int componentCount,
		unsigned int basicDataType,
		bool normalize,
		int dataStride,
		int dataOffset);
	void Active();
};