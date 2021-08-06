#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
typedef void* XVulkanHandle;
enum XBufferObjectType{
	kXBufferObjectTypeVertexBuffer,
	kXBufferObjectTypeIndexBuffer,
	kXBufferObjectTypeUniformBuffer,
	kXBufferObjectTypeCount
};
struct XBufferObject {
	VkBuffer mBuffer;
	VkDeviceMemory mMemory;
	XBufferObjectType mType;
	XBufferObject();
	virtual ~XBufferObject();
	void OnSetSize();
	void SubmitData(const void*data, int size);
	virtual int GetSize();
};
struct XMatrix4x4f {
	float mData[16];
	XMatrix4x4f() {
		memset(mData, 0, sizeof(float) * 16);
		mData[0] = 1.0f;
		mData[5] = 1.0f;
		mData[10] = 1.0f;
		mData[15] = 1.0f;
	}
};
struct XVector4f {
	float mData[4];
};
struct XVertexData{
	float mPosition[4];
	float mTexcoord[4];
	float mNormal[4];
	float mTangent[4];
	void SetPosition(float x, float y, float z, float w = 1.0f);
	void SetTexcoord(float x, float y, float z = 0.0f, float w = 0.0f);
	void SetNormal(float x, float y, float z, float w = 0.0f);
	void SetTangent(float x, float y, float z, float w = 0.0f);
	static const VkVertexInputBindingDescription & BindingDescription();
	static const std::vector<VkVertexInputAttributeDescription> & AttributeDescriptions();
};
enum XUniformBufferType {
	kXUniformBufferTypeMatrix,
	kXUniformBufferTypeVector,
	kXUniformBufferTypeCount
};
struct XUniformBuffer {
	VkBuffer mBuffer;
	VkDeviceMemory mMemory;
	XUniformBufferType mType;
	std::vector<XMatrix4x4f> mMatrices;
	std::vector<XVector4f> mVector4s;
	XUniformBuffer();
	~XUniformBuffer();
};
struct XFixedPipeline {
	VkPipeline mPipeline;
	VkPipelineLayout mPipelineLayout;
	VkDescriptorSetLayout *mDescriptorSetLayout;
	VkPipelineShaderStageCreateInfo *mShaderStages;
	int mShaderStageCount, mDescriptorSetLayoutCount;
	VkRenderPass mRenderPass;
	VkSampleCountFlagBits mSampleCount;
	VkPipelineInputAssemblyStateCreateInfo mInputAssetmlyState;
	VkPipelineViewportStateCreateInfo mViewportState;
	VkViewport mViewport;
	VkRect2D mScissor;
	VkPipelineRasterizationStateCreateInfo mRasterizer;
	VkPipelineDepthStencilStateCreateInfo mDepthStencilState;
	VkPipelineMultisampleStateCreateInfo mMultisampleState;
	std::vector<VkPipelineColorBlendAttachmentState> mColorBlendAttachmentStates;
	VkPipelineColorBlendStateCreateInfo mColorBlendState;
	XVector4f mPushConstants[16];
	int mPushConstantCount;
	VkShaderStageFlags mPushConstantShaderStage;
	float mDepthConstantFactor, mDepthClamp, mDepthSlopeFactor;
	XFixedPipeline();
	~XFixedPipeline();
	void CleanUp();
};
struct XProgram {
	VkPipelineShaderStageCreateInfo mShaderStage[2];
	int mShaderStagetCount;
	VkShaderModule mVertexShader, mFragmentShader;
	VkDescriptorSetLayout mDescriptorSetLayout;
	std::vector<VkDescriptorSetLayoutBinding> mDescriptorSetLayoutBindings;
	std::vector<VkDescriptorPoolSize> mDescriptorPoolSize;
	std::vector<VkWriteDescriptorSet> mWriteDescriptorSet;
	VkDescriptorSet mDescriptorSet;
	VkDescriptorPool mDescriptorPool;
	XUniformBuffer mVertexShaderMatrixUniformBuffer;
	XUniformBuffer mFragmentShaderMatrixUniformBuffer;
	XUniformBuffer mVertexShaderVectorUniformBuffer;
	XUniformBuffer mFragmentShaderVectorUniformBuffer;
	XFixedPipeline mFixedPipeline;
	XProgram();
	~XProgram();
};
struct XTexture {
	VkImage mImage;
	VkDeviceMemory mMemory;
	VkImageView mImageView;
	VkSampler mSampler;
	VkImageLayout mInitLayout;
	VkImageLayout mTargetLayout;
	VkPipelineStageFlags mSrcStage;
	VkPipelineStageFlags mTargetStage;
	VkImageAspectFlags mImageAspectFlag;
	VkFormat mFormat;
	VkFilter mMinFilter, mMagFilter;
	VkSamplerAddressMode mWrapU, mWrapV, mWrapW;
	VkBool32 mbEnableAnisotropy;
	float mMaxAnisotropy;
	XTexture(VkImageAspectFlags image_aspect = VK_IMAGE_ASPECT_COLOR_BIT);
	virtual ~XTexture();
};
struct XSystemFrameBuffer {
	VkFramebuffer mFrameBuffer;
	VkImageView mColorBuffer;
	VkImageView mDepthBuffer;
	VkImageView mResolveBuffer;
	VkSampleCountFlagBits mSampleCount;
	XSystemFrameBuffer();
	~XSystemFrameBuffer();
};
VkResult xGenBuffer(VkBuffer&buffer, VkDeviceMemory&buffermemory, VkDeviceSize size, VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties);
void xBufferSubData(VkBuffer buffer, VkBufferUsageFlags usage, const void * data, VkDeviceSize size);
#define xGenVertexBuffer(size,buffer,buffermemory) \
	xGenBuffer(buffer,buffermemory,size,VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
#define xGenIndexBuffer(size,buffer,buffermemory) \
	xGenBuffer(buffer,buffermemory,size,VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_INDEX_BUFFER_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
#define xBufferSubVertexData(buffer,data,size) \
	xBufferSubData(buffer,VK_BUFFER_USAGE_TRANSFER_SRC_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,data,size);
#define xBufferSubIndexData(buffer,data,size) \
	xBufferSubData(buffer,VK_BUFFER_USAGE_TRANSFER_SRC_BIT|VK_BUFFER_USAGE_INDEX_BUFFER_BIT,data,size);
uint32_t xGetMemoryType(uint32_t type_filters, VkMemoryPropertyFlags properties);
void xBeginOneTimeCommandBuffer(VkCommandBuffer*commandbuffer);
void xEndOneTimeCommandBuffer(VkCommandBuffer commandbuffer);
void xGenCommandBuffer(VkCommandBuffer*commandbuffer, int count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
void xWaitForCommandFinish(VkCommandBuffer commandbuffer);
void xCreateShader(VkShaderModule&shader, unsigned char *code, int code_len);
void xAttachVertexShader(XProgram*program, VkShaderModule shader);
void xAttachFragmentShader(XProgram*program, VkShaderModule shader);
void xInitDescriptorSetLayout(XProgram*program);
void xInitDescriptorPool(XProgram*program);
void xInitDescriptorSet(XProgram*program);
void xConfigUniformBuffer(XVulkanHandle param, int bingding, XBufferObject *ubo, VkShaderStageFlags shader_stage);
void xGenImage(XTexture*texture, uint32_t w, uint32_t h, VkFormat f,
	VkImageUsageFlags usage, VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT, int mipmap = 1);
void xGenImageCube(XTexture*texture, uint32_t w, uint32_t h, VkFormat f,
	VkImageUsageFlags usage, VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT, int mipmap = 1);
void xSubmitImage2D(XTexture*texture, int width, int height, const void *pixel);
void xSubmitImageCube(XTexture*texture, int width, int height, const void *pixel);
void xSetImageLayout(VkCommandBuffer commandbuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,
	VkImageSubresourceRange subresourcerange, VkPipelineStageFlags src = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
	VkPipelineStageFlags dst = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
void xGenImageView2D(XTexture*texture, int mipmap = 1);
void xGenImageViewCube(XTexture*texture, int mipmap = 1);
void xGenSampler(XTexture*texture);
void xInitDefaultTexture();
void xVulkanCleanUp();
void xConfigSampler2D(XProgram*program, int binding, VkImageView imageview, VkSampler sampler,
	VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
unsigned char *LoadImageFromFile(const char *path, int &width, int &height, int &channel, int force_channel
	, bool flipY = false);
XTexture*xGetDefaultTexture();
void xRebindUniformBuffer(XProgram*program, int binding, XUniformBuffer*ubo);
void xRebindSampler(XProgram*program, int binding, VkImageView iv, VkSampler s,
	VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
VkCommandBuffer xBeginRendering(VkCommandBuffer commandbuffer=nullptr);
void xEndRendering();
void xSwapBuffers(VkCommandBuffer commandbuffer=nullptr);
void xSetColorAttachmentCount(XFixedPipeline*pipeline, int count);
void xEnableBlend(XFixedPipeline*pipeline, int attachment, VkBool32 enable);
void xBlend(XFixedPipeline*p, int attachment, VkBlendFactor s_c, VkBlendFactor s_a,
	VkBlendFactor d_c, VkBlendFactor d_a);
void xBlendOp(XFixedPipeline*p, int attachment, VkBlendOp color, VkBlendOp alpha);
void xPolygonMode(XFixedPipeline*p, VkPolygonMode mode);
void xDisableRasterizer(XFixedPipeline*p, VkBool32 disable);
void xEnableDepthTest(XFixedPipeline*p, VkBool32 enable);
void xInitPipelineLayout(XFixedPipeline*p);
void xCreateFixedPipeline(XFixedPipeline*p);
void xSetDynamicState(XFixedPipeline*p, VkCommandBuffer commandbuffer);
void xInitSystemFrameBuffer();
XVulkanHandle xGetSystemFrameBuffer(int index);
int xGetSystemFrameBufferCount();
void xViewport(int width, int height);
VkRenderPass xGetGlobalRenderPass();
void xInitCommandPool();
VkCommandPool xGetCommandPool();
void xInitSemaphores();
VkSemaphore xGetReadyToRenderSemaphore();
VkSemaphore xGetReadyToPresentSemaphore();
void xInitSwapChain();
VkSwapchainKHR xGetSwapChain();
VkFormat xGetSwapChainImageFormat();
VkDevice xGetVulkanDevice();
void xInitVulkanDevice();
VkQueue xGetGraphicQueue();
VkQueue xGetPresentQueue();
void xInitVulkanPhysicalDevice();
VkPhysicalDevice xGetVulkanPhysicalDevice();
int xGetGraphicQueueFamily();
int xGetPresentQueueFamily();
VkSampleCountFlagBits xGetMaxMSAASampleCount();
void xInitVulkan(void*param, int width, int height);
int xGetViewportWidth();
int xGetViewportHeight();
VkSampleCountFlagBits xGetGlobalFrameBufferSampleCount();
VkFramebuffer xAquireRenderTarget();
uint32_t xGetCurrenRenderTargetIndex();
unsigned char * LoadFileContent(const char *path, int &filesize);