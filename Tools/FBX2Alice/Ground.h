#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "Material.h"
class Ground {
public:
	VertexBuffer*mVertexBuffer;
	IndexBuffer*mIndexBuffer;
	Material*mMaterial;
public:
	~Ground();
	void Init();
	void Draw(VkCommandBuffer commandbuffer);
	void SetMaterial(Material*material);
};