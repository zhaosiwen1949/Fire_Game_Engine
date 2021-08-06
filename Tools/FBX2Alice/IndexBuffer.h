#pragma once
#include "XVulkan.h"
class IndexBuffer : public XBufferObject {
public:
	int mIndexCount;
	int mCurrentIndex;
	unsigned int *mIndexes;
public:
	IndexBuffer();
	~IndexBuffer();
	void SetSize(int count);
	int GetSize();
	void AppendIndex(int index);
	void SubmitData();
};