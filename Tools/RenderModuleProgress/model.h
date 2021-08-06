#pragma once
#include "AliceGL.h"
class Model {
public:
	void * mData;
	int mVertexCount;
public:
	Model();
	void Init(const char*modelPath);
};