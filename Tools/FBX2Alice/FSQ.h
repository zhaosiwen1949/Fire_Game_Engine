#pragma once
#include "VertexBuffer.h"
#include "Texture2D.h"
#include "Material.h"
class FSQ {
public:
	VertexBuffer*mVertexBuffer;
	Material*mMaterial;
public:
	~FSQ();
	void Init();
	void Draw(VkCommandBuffer commandbuffer);
};