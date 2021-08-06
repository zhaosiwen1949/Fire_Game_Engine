#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "Material.h"
class Model {
public:
	VertexBuffer*mVertexBuffer;
	IndexBuffer*mIndexBuffer;
	Material*mMaterial;
public:
	~Model();
	void Init(const char *path);
	void Draw(VkCommandBuffer commandbuffer);
	void SetMaterial(Material*material);
};