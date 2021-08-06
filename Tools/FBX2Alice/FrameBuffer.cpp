#include "FrameBuffer.h"
FrameBuffer::FrameBuffer() {
	mFBO = 0;
	mRenderPass = 0;
	mDepthBuffer = nullptr;
}
FrameBuffer::~FrameBuffer() {
	for (auto iter=mAttachments.begin();iter!=mAttachments.end();++iter){
		delete *iter;
	}
	if (mRenderPass != 0) {
		vkDestroyRenderPass(xGetVulkanDevice(), mRenderPass, nullptr);
	}
	if (mFBO!=0){
		vkDestroyFramebuffer(xGetVulkanDevice(), mFBO, nullptr);
	}
}
void FrameBuffer::SetSize(uint32_t width, uint32_t height) {
	mWidth = width;
	mHeight = height;
}
void FrameBuffer::AttachColorBuffer(VkFormat format /* = VK_FORMAT_R8G8B8A8_UNORM */) {
	Texture2D*color_buffer = new Texture2D;
	color_buffer->mFormat = format;
	xGenImage(color_buffer, mWidth, mHeight, format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
		VK_IMAGE_USAGE_SAMPLED_BIT);
	xGenImageView2D(color_buffer);
	xGenSampler(color_buffer);
	mColorBufferCount++;
	mAttachments.push_back(color_buffer);
	VkClearValue cv;
	cv.color = { 0.0f,0.0f,0.0f,0.0f };
	mClearValues.push_back(cv);
}
void FrameBuffer::AttachDepthBuffer() {
	Texture2D *depth_buffer = new Texture2D(VK_IMAGE_ASPECT_DEPTH_BIT);
	depth_buffer->mFormat = VK_FORMAT_D24_UNORM_S8_UINT;
	xGenImage(depth_buffer, mWidth, mHeight, depth_buffer->mFormat, 
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	xGenImageView2D(depth_buffer);
	xGenSampler(depth_buffer);
	mDepthBufferIndex = mAttachments.size();
	mAttachments.push_back(depth_buffer);
	VkClearValue cv;
	cv.depthStencil = {1.0f,0};
	mClearValues.push_back(cv);
	mDepthBuffer = depth_buffer;
}
void FrameBuffer::Finish() {
	std::vector<VkAttachmentDescription> attachments;
	std::vector<VkAttachmentReference> colorattachment_refences;
	VkAttachmentReference depthattachment_ref = {};
	std::vector<VkImageView> render_targets;
	render_targets.resize(mAttachments.size());
	attachments.resize(mAttachments.size());
	int color_buffer_count = 0;
	for (size_t i=0;i<mAttachments.size();++i){
		Texture2D*texture = mAttachments[i];
		if (texture->mImageAspectFlag==VK_IMAGE_ASPECT_COLOR_BIT){
			color_buffer_count++;
		}else if (texture->mImageAspectFlag==VK_IMAGE_ASPECT_DEPTH_BIT){

		}
		render_targets[i] = texture->mImageView;
	}
	colorattachment_refences.resize(color_buffer_count);
	int color_buffer_index = 0;
	int attachment_point = 0;
	for (size_t i=0;i<mAttachments.size();++i){
		Texture2D*texture = mAttachments[i];
		if (texture->mImageAspectFlag==VK_IMAGE_ASPECT_COLOR_BIT){
			attachments[i] = {
				0,
				texture->mFormat,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			};
			colorattachment_refences[color_buffer_index++] = {
				uint32_t(attachment_point++),VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			};
		}else if (texture->mImageAspectFlag==VK_IMAGE_ASPECT_DEPTH_BIT){
			attachments[i] = {
				0,
				texture->mFormat,
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			};
			depthattachment_ref.attachment = attachment_point++;
			depthattachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
	}

	VkSubpassDescription subpasses = {};
	subpasses.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpasses.colorAttachmentCount = colorattachment_refences.size();
	subpasses.pColorAttachments = colorattachment_refences.data();
	subpasses.pDepthStencilAttachment = &depthattachment_ref;

	VkRenderPassCreateInfo rpci = {};
	rpci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpci.attachmentCount = attachments.size();
	rpci.pAttachments = attachments.data();
	rpci.subpassCount = 1;
	rpci.pSubpasses = &subpasses;
	vkCreateRenderPass(xGetVulkanDevice(), &rpci, nullptr,&mRenderPass);
	VkFramebufferCreateInfo fbci = {};
	fbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fbci.pAttachments = render_targets.data();
	fbci.attachmentCount = render_targets.size();
	fbci.width = mWidth;
	fbci.height = mHeight;
	fbci.layers = 1;
	fbci.renderPass = mRenderPass;
	vkCreateFramebuffer(xGetVulkanDevice(), &fbci, nullptr, &mFBO);
}
void FrameBuffer::SetClearColor(int index, float r, float g, float b, float a) {
	mClearValues[index].color = {r,g,b,a};
}
void FrameBuffer::SetClearDepthStencil(float depth, uint32_t stencil) {
	mClearValues[mDepthBufferIndex].depthStencil = { depth,stencil };
}
VkCommandBuffer FrameBuffer::BeginRendering(VkCommandBuffer commandbuffer /* = nullptr */) {
	VkCommandBuffer cmd;
	if (commandbuffer != nullptr) {
		cmd = commandbuffer;
	}
	else {
		xBeginOneTimeCommandBuffer(&cmd);
	}
	VkFramebuffer render_target = mFBO;
	VkRenderPass render_pass = mRenderPass;

	VkRenderPassBeginInfo rpbi = {};
	rpbi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpbi.framebuffer = render_target;
	rpbi.renderPass = render_pass;
	rpbi.renderArea.offset = { 0,0 };
	rpbi.renderArea.extent = { uint32_t(xGetViewportWidth()),uint32_t(xGetViewportHeight()) };
	rpbi.clearValueCount = mClearValues.size();
	rpbi.pClearValues = mClearValues.data();
	vkCmdBeginRenderPass(cmd, &rpbi, VK_SUBPASS_CONTENTS_INLINE);
	return cmd;
}