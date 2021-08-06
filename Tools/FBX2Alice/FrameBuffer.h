#pragma once
#include "VertexBuffer.h"
#include "Texture2D.h"
#include "Material.h"
class FrameBuffer {
public:
	VkFramebuffer mFBO;
	VkRenderPass mRenderPass;
	uint32_t mWidth, mHeight;
	std::vector<Texture2D*>mAttachments;
	std::vector<VkClearValue> mClearValues;
	int mColorBufferCount;
	int mDepthBufferIndex;
	Texture2D*mDepthBuffer;
public:
	FrameBuffer();
	~FrameBuffer();
	void SetSize(uint32_t width, uint32_t height);
	void AttachColorBuffer(VkFormat format = VK_FORMAT_R8G8B8A8_UNORM);
	void AttachDepthBuffer();
	void Finish();
	void SetClearColor(int index, float r, float g, float b, float a);
	void SetClearDepthStencil(float depth, uint32_t stencil);
	VkCommandBuffer BeginRendering(VkCommandBuffer commandbuffer = nullptr);
};