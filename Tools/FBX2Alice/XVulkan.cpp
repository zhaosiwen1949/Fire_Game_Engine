#include "XVulkan.h"
#include "stb_image_aug.h"
#pragma comment(lib,"vulkan-1.lib")
#define MSAA 1
static XTexture*sDefaultTexture = nullptr;
static VkCommandBuffer sMainCommandBuffer;
static float sClearColor[] = {0.1f,0.4f,0.6f,1.0f};
static float sClearDepth = 1.0f;
static uint32_t sClearStencil = 0;
static VkRenderPass sRenderPass = 0;
static XTexture *sColorBuffer = nullptr, *sDepthBuffer = nullptr;
static int sFrameBufferCount = 0;
static int sViewportWidth = 0, sViewportHeight = 0;
static XSystemFrameBuffer *sSystemFrameBuffer = nullptr;
static VkCommandPool sCommandPool;
static VkSemaphore sReadyToRender, sReadyToPresent;
static VkSwapchainKHR sSwapChain;
static VkFormat sSwapChainImageFormat=VK_FORMAT_B8G8R8A8_UNORM;
static VkExtent2D sSwapChainExtent;
static std::vector<VkImage> sSwapChainImages;
static std::vector<VkImageView> sSwapChainImageViews;
static VkDevice sVulkanDevice;
static VkQueue sGraphicQueue, sPresentQueue;
static VkPhysicalDevice sPhysicalDevice;
static VkSampleCountFlagBits sMaxSampleCount;
static int sGraphicQueueFamily, sPresentQueueFamily;
static void* sWindowHWND = nullptr;
static VkInstance sVulkanInstance;
static VkSurfaceKHR sVulkanSurface;
static VkDebugReportCallbackEXT sVulkanDebugger;
static PFN_vkCreateDebugReportCallbackEXT __vkCreateDebugReportCallback = nullptr;
static PFN_vkDestroyDebugReportCallbackEXT __vkDestroyDebugReportCallback = nullptr;
static PFN_vkCreateWin32SurfaceKHR __vkCreateWin32SurfaceKHR = nullptr;
static uint32_t sCurrentRenderFrameBufferIndex = 0;
XBufferObject::XBufferObject() {
	mBuffer = 0;
	mMemory = 0;
}
XBufferObject::~XBufferObject() {
	if (mBuffer!=0){
		vkDestroyBuffer(sVulkanDevice, mBuffer, nullptr);
	}
	if (mMemory!=0){
		vkFreeMemory(sVulkanDevice, mMemory, nullptr);
	}
}
void XBufferObject::OnSetSize() {
	if (mType==kXBufferObjectTypeVertexBuffer){
		xGenVertexBuffer(GetSize(), mBuffer, mMemory);
	}else if (mType==kXBufferObjectTypeIndexBuffer){
		xGenIndexBuffer(GetSize(), mBuffer, mMemory);
	}else if (mType==kXBufferObjectTypeUniformBuffer){
		xGenBuffer(mBuffer, mMemory,GetSize(), 
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	}
}
void XBufferObject::SubmitData(const void*data, int size) {
	if (mType==kXBufferObjectTypeVertexBuffer){
		xBufferSubVertexData(mBuffer, data, size);
	}else if (mType==kXBufferObjectTypeIndexBuffer){
		xBufferSubIndexData(mBuffer, data, size);
	}
	else if (mType == kXBufferObjectTypeUniformBuffer) {
		void *dst;
		vkMapMemory(sVulkanDevice, mMemory, 0, size, 0, &dst);
		memcpy(dst, data, size);
		vkUnmapMemory(sVulkanDevice, mMemory);
	}
}
int XBufferObject::GetSize() {
	return 0;
}
XUniformBuffer::XUniformBuffer() {
	mBuffer = 0;
	mMemory = 0;
}
XUniformBuffer::~XUniformBuffer() {
	if (mBuffer!=0){
		vkDestroyBuffer(sVulkanDevice, mBuffer, nullptr);
	}
	if (mMemory!=0){
		vkFreeMemory(sVulkanDevice, mMemory, nullptr);
	}
}
XProgram::XProgram() {
	mShaderStagetCount = 0;
	mVertexShader = 0;
	mFragmentShader = 0;
	mDescriptorPool = 0;
	mDescriptorSetLayout = 0;
	mDescriptorSet = 0;
	memset(mShaderStage, 0, sizeof(VkPipelineShaderStageCreateInfo)*2);
}
XProgram::~XProgram() {
	if (mVertexShader!=0){
		vkDestroyShaderModule(sVulkanDevice, mVertexShader, nullptr);
	}
	if (mFragmentShader != 0) {
		vkDestroyShaderModule(sVulkanDevice, mFragmentShader, nullptr);
	}
	if (mDescriptorPool!=0){
		vkDestroyDescriptorPool(sVulkanDevice, mDescriptorPool, nullptr);
	}
	if (mDescriptorSetLayout!=0){
		vkDestroyDescriptorSetLayout(sVulkanDevice, mDescriptorSetLayout, nullptr);
	}
	for (int i=0;i<mWriteDescriptorSet.size();++i){
		VkWriteDescriptorSet*wds = &mWriteDescriptorSet[i];
		if (wds->pBufferInfo!=nullptr){
			delete wds->pBufferInfo;
		}
		if (wds->pImageInfo!=nullptr){
			delete wds->pImageInfo;
		}
	}
}
XTexture::XTexture(VkImageAspectFlags image_aspect) {
	mImageAspectFlag = image_aspect;
	mImage = 0;
	mImageView = 0;
	mMemory = 0;
	mSampler = 0;
	mInitLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	mTargetLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	mMinFilter = VK_FILTER_LINEAR;
	mMagFilter = VK_FILTER_LINEAR;
	mWrapU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	mWrapV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	mWrapW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	mbEnableAnisotropy = VK_FALSE;
	mMaxAnisotropy = 0.0f;
}
XTexture::~XTexture() {
	if (mMemory!=0){
		vkFreeMemory(sVulkanDevice, mMemory, nullptr);
	}
	if (mImageView!=0){
		vkDestroyImageView(sVulkanDevice, mImageView, nullptr);
	}
	if (mImage!=0){
		vkDestroyImage(sVulkanDevice, mImage, nullptr);
	}
	if (mSampler!=0){
		vkDestroySampler(sVulkanDevice, mSampler, nullptr);
	}
}
XFixedPipeline::XFixedPipeline() {
	mPipelineLayout = 0;
	mPipeline = 0;
	mInputAssetmlyState = {};
	mInputAssetmlyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	mInputAssetmlyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	mInputAssetmlyState.primitiveRestartEnable = VK_FALSE;
	mViewport = {};
	mScissor = {};
	mViewportState = {};
	mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	mViewportState.viewportCount = 1;
	mViewportState.pViewports = &mViewport;
	mViewportState.scissorCount = 1;
	mViewportState.pScissors = &mScissor;
	mRasterizer = {};
	mRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	mRasterizer.depthClampEnable = VK_FALSE;
	mRasterizer.rasterizerDiscardEnable = VK_FALSE;
	mRasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	mRasterizer.lineWidth = 1.0f;
	mRasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	mRasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	mRasterizer.depthBiasEnable = VK_TRUE;
	mRasterizer.depthBiasConstantFactor = 0.0f;
	mRasterizer.depthBiasClamp = 0.0f;
	mRasterizer.depthBiasSlopeFactor = 0.0f;
	mDepthStencilState = {};
	mDepthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	mDepthStencilState.depthTestEnable = VK_TRUE;
	mDepthStencilState.depthWriteEnable = VK_TRUE;
	mDepthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	mDepthStencilState.depthBoundsTestEnable = VK_FALSE;
	mDepthStencilState.minDepthBounds = 0.0f;
	mDepthStencilState.maxDepthBounds = 1.0f;
	mDepthStencilState.stencilTestEnable = VK_FALSE;
	mDepthStencilState.front = {};
	mDepthStencilState.back = {};
	mMultisampleState = {};
	mMultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	mMultisampleState.sampleShadingEnable = VK_TRUE;
	mMultisampleState.rasterizationSamples = xGetGlobalFrameBufferSampleCount();
	mMultisampleState.minSampleShading = 1.0f;
	mMultisampleState.pSampleMask = nullptr;
	mMultisampleState.alphaToCoverageEnable = VK_FALSE;
	mMultisampleState.alphaToOneEnable = VK_FALSE;
	mColorBlendState = {};
	mColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	mColorBlendState.logicOpEnable = VK_FALSE;
	mColorBlendState.logicOp = VK_LOGIC_OP_COPY;
	mDescriptorSetLayout = nullptr;
	mShaderStages = nullptr;
	mShaderStageCount = 0;
	mDescriptorSetLayoutCount = 0;
	mRenderPass = 0;
	mSampleCount = xGetGlobalFrameBufferSampleCount();
	mPushConstantShaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
	mPushConstantCount = 8;
	mDepthConstantFactor = 0.0f;
	mDepthClamp = 0.0f;
	mDepthSlopeFactor = 0.0f;
}
XFixedPipeline::~XFixedPipeline() {
	CleanUp();
}
void XFixedPipeline::CleanUp() {
	if (mPipeline != 0) {
		vkDestroyPipeline(sVulkanDevice, mPipeline, nullptr);
	}
	if (mPipelineLayout!=0){
		vkDestroyPipelineLayout(sVulkanDevice, mPipelineLayout, nullptr);
	}
}
void XVertexData::SetPosition(float x, float y, float z, float w /* = 1.0f */) {
	mPosition[0] = x;
	mPosition[1] = y;
	mPosition[2] = z;
	mPosition[3] = w;
}
void XVertexData::SetTexcoord(float x, float y, float z, float w /* = 1.0f */) {
	mTexcoord[0] = x;
	mTexcoord[1] = y;
	mTexcoord[2] = z;
	mTexcoord[3] = w;
}
void XVertexData::SetNormal(float x, float y, float z, float w /* = 1.0f */) {
	mNormal[0] = x;
	mNormal[1] = y;
	mNormal[2] = z;
	mNormal[3] = w;
}
void XVertexData::SetTangent(float x, float y, float z, float w /* = 1.0f */) {
	mTangent[0] = x;
	mTangent[1] = y;
	mTangent[2] = z;
	mTangent[3] = w;
}
const VkVertexInputBindingDescription&XVertexData::BindingDescription() {
	static VkVertexInputBindingDescription binding_description = {};
	binding_description.binding = 0;
	binding_description.stride = sizeof(XVertexData);
	binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return binding_description;
}
const std::vector<VkVertexInputAttributeDescription>&XVertexData::AttributeDescriptions() {
	static std::vector<VkVertexInputAttributeDescription> attributesDescriptions;
	attributesDescriptions.resize(4);
	attributesDescriptions[0].binding = 0;
	attributesDescriptions[0].location = 0;
	attributesDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributesDescriptions[0].offset = 0;

	attributesDescriptions[1].binding = 0;
	attributesDescriptions[1].location = 1;
	attributesDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributesDescriptions[1].offset = sizeof(float) * 4;

	attributesDescriptions[2].binding = 0;
	attributesDescriptions[2].location = 2;
	attributesDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributesDescriptions[2].offset = sizeof(float) * 8;

	attributesDescriptions[3].binding = 0;
	attributesDescriptions[3].location = 3;
	attributesDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributesDescriptions[3].offset = sizeof(float) * 12;
	return attributesDescriptions;
}
XSystemFrameBuffer::XSystemFrameBuffer() {
	mFrameBuffer = 0;
	mColorBuffer = 0;
	mDepthBuffer = 0;
	mResolveBuffer = 0;
	mSampleCount = VK_SAMPLE_COUNT_1_BIT;
}
XSystemFrameBuffer::~XSystemFrameBuffer() {
	if (mFrameBuffer!=0){
		vkDestroyFramebuffer(sVulkanDevice, mFrameBuffer, nullptr);
	}
}
VkResult xGenBuffer(VkBuffer&buffer, VkDeviceMemory&buffermemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
	VkBufferCreateInfo bufferinfo = {};
	bufferinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferinfo.size = size;
	bufferinfo.usage = usage;
	VkResult ret = vkCreateBuffer(sVulkanDevice, &bufferinfo, nullptr, &buffer);
	if (ret!=VK_SUCCESS){
		printf("failed to create buffer\n");
		return ret;
	}
	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(sVulkanDevice, buffer, &requirements);
	VkMemoryAllocateInfo memoryallocinfo = {};
	memoryallocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryallocinfo.allocationSize = requirements.size;
	memoryallocinfo.memoryTypeIndex = xGetMemoryType(requirements.memoryTypeBits, properties);
	ret = vkAllocateMemory(sVulkanDevice, &memoryallocinfo, nullptr, &buffermemory);
	if (ret!=VK_SUCCESS){
		printf("failed to alloc memory\n");
		return ret;
	}
	vkBindBufferMemory(sVulkanDevice, buffer, buffermemory, 0);
	return VK_SUCCESS;
}
void xBufferSubData(VkBuffer buffer, VkBufferUsageFlags usage, const void * data, VkDeviceSize size) {
	VkBuffer tempbuffer;
	VkDeviceMemory tempmemory;
	xGenBuffer(tempbuffer, tempmemory, size, usage,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	void*host_memory;
	vkMapMemory(sVulkanDevice, tempmemory, 0, size, 0, &host_memory);
	memcpy(host_memory, data, (size_t)size);
	vkUnmapMemory(sVulkanDevice, tempmemory);

	VkCommandBuffer commandbuffer;
	xBeginOneTimeCommandBuffer(&commandbuffer);
	VkBufferCopy copy = {0,0,size};
	vkCmdCopyBuffer(commandbuffer, tempbuffer, buffer, 1, &copy);
	xEndOneTimeCommandBuffer(commandbuffer);

	vkDestroyBuffer(sVulkanDevice, tempbuffer, nullptr);
	vkFreeMemory(sVulkanDevice, tempmemory, nullptr);
}
uint32_t xGetMemoryType(uint32_t type_filters, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(sPhysicalDevice, &memory_properties);
	for (uint32_t i=0;i<memory_properties.memoryTypeCount;++i){
		uint32_t flag = 1 << i;
		if ((flag&type_filters)&&(memory_properties.memoryTypes[i].propertyFlags&properties)==properties){
			return i;
		}
	}
	return 0;
}
void xBeginOneTimeCommandBuffer(VkCommandBuffer*commandbuffer) {
	xGenCommandBuffer(commandbuffer, 1);
	VkCommandBufferBeginInfo cbbi = {};
	cbbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cbbi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(*commandbuffer, &cbbi);
}
void xEndOneTimeCommandBuffer(VkCommandBuffer commandbuffer) {
	vkEndCommandBuffer(commandbuffer);
	xWaitForCommandFinish(commandbuffer);
	vkFreeCommandBuffers(sVulkanDevice, sCommandPool, 1, &commandbuffer);
}
void xGenCommandBuffer(VkCommandBuffer*commandbuffer, int count, VkCommandBufferLevel level) {
	VkCommandBufferAllocateInfo cbai = {};
	cbai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbai.level = level;
	cbai.commandPool = sCommandPool;
	cbai.commandBufferCount = count;
	vkAllocateCommandBuffers(sVulkanDevice, &cbai, commandbuffer);
}
void xWaitForCommandFinish(VkCommandBuffer commandbuffer) {
	VkSubmitInfo submitinfo = {};
	submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitinfo.commandBufferCount = 1;
	submitinfo.pCommandBuffers = &commandbuffer;
	VkFenceCreateInfo fci = {};
	fci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	VkFence fence;
	vkCreateFence(sVulkanDevice, &fci, nullptr, &fence);
	vkQueueSubmit(sGraphicQueue, 1, &submitinfo, fence);
	vkWaitForFences(sVulkanDevice, 1, &fence, VK_TRUE,1000000000);
	vkDestroyFence(sVulkanDevice, fence, nullptr);
}
void xCreateShader(VkShaderModule&shader, unsigned char *code, int code_len) {
	VkShaderModuleCreateInfo smci = {};
	smci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	smci.codeSize = code_len;
	smci.pCode = (uint32_t*)code;
	vkCreateShaderModule(sVulkanDevice, &smci, nullptr, &shader);
}
void xAttachVertexShader(XProgram*program, VkShaderModule shader) {
	program->mShaderStage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	program->mShaderStage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	program->mShaderStage[0].module = shader;
	program->mShaderStage[0].pName = "main";
	program->mVertexShader = shader;
}
void xAttachFragmentShader(XProgram*program, VkShaderModule shader) {
	program->mShaderStage[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	program->mShaderStage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	program->mShaderStage[1].module = shader;
	program->mShaderStage[1].pName = "main";
	program->mFragmentShader = shader;
}
void xInitDescriptorSetLayout(XProgram*program) {
	VkDescriptorSetLayoutCreateInfo dslci = {};
	dslci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	dslci.bindingCount = uint32_t(program->mDescriptorSetLayoutBindings.size());
	dslci.pBindings = program->mDescriptorSetLayoutBindings.data();
	vkCreateDescriptorSetLayout(sVulkanDevice, &dslci, nullptr, &program->mDescriptorSetLayout);
}
void xInitDescriptorPool(XProgram*program) {
	if (program->mDescriptorPoolSize.empty()){
		return;
	}
	VkDescriptorPoolCreateInfo dpci = {};
	dpci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	dpci.poolSizeCount = uint32_t(program->mDescriptorPoolSize.size());
	dpci.pPoolSizes = program->mDescriptorPoolSize.data();
	dpci.maxSets = 1;
	vkCreateDescriptorPool(sVulkanDevice, &dpci, nullptr, &program->mDescriptorPool);
}
void xInitDescriptorSet(XProgram*program) {
	if (program->mWriteDescriptorSet.empty()){
		return;
	}
	VkDescriptorSetAllocateInfo dsai = {};
	dsai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	dsai.descriptorPool = program->mDescriptorPool;
	dsai.descriptorSetCount = 1;
	dsai.pSetLayouts = &program->mDescriptorSetLayout;
	vkAllocateDescriptorSets(sVulkanDevice, &dsai, &program->mDescriptorSet);
	for (int i=0;i<program->mWriteDescriptorSet.size();++i){
		program->mWriteDescriptorSet[i].dstSet = program->mDescriptorSet;
	}
	vkUpdateDescriptorSets(sVulkanDevice, uint32_t(program->mWriteDescriptorSet.size()), program->mWriteDescriptorSet.data(),0,nullptr);
}
void xConfigUniformBuffer(XVulkanHandle param, int bingding, XBufferObject *ubo, VkShaderStageFlags shader_stage) {
	XProgram*program = (XProgram*)param;
	VkDescriptorSetLayoutBinding dslb = {};
	dslb.binding = bingding;
	dslb.descriptorCount = 1;
	dslb.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	dslb.stageFlags = shader_stage;
	program->mDescriptorSetLayoutBindings.push_back(dslb);
	VkDescriptorPoolSize dps = {};
	dps.descriptorCount = 1;
	dps.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	program->mDescriptorPoolSize.push_back(dps);
	VkDescriptorBufferInfo *bufferinfo = new VkDescriptorBufferInfo;
	bufferinfo->offset = 0;
	bufferinfo->buffer = ubo->mBuffer;
	bufferinfo->range = ubo->GetSize();
	VkWriteDescriptorSet descriptorwriter = {};
	descriptorwriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorwriter.dstSet = program->mDescriptorSet;
	descriptorwriter.dstBinding = bingding;
	descriptorwriter.dstArrayElement = 0;
	descriptorwriter.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorwriter.descriptorCount = 1;
	descriptorwriter.pBufferInfo = bufferinfo;
	program->mWriteDescriptorSet.push_back(descriptorwriter);
}
void xGenImage(XTexture*texture, uint32_t w, uint32_t h, VkFormat f,
	VkImageUsageFlags usage, VkSampleCountFlagBits sample_count, int mipmap) {
	VkImageCreateInfo ici = {};
	ici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ici.imageType = VK_IMAGE_TYPE_2D;
	ici.extent = {w,h,1};
	ici.mipLevels = mipmap;
	ici.arrayLayers = 1;
	ici.format = f;
	ici.initialLayout = texture->mInitLayout;
	ici.usage = usage;
	ici.samples = sample_count;
	if (vkCreateImage(sVulkanDevice,&ici,nullptr,&texture->mImage)!=VK_SUCCESS){
		printf("failed to create image\n");
	}
	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(sVulkanDevice, texture->mImage, &memory_requirements);
	VkMemoryAllocateInfo mai = {};
	mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mai.allocationSize = memory_requirements.size;
	mai.memoryTypeIndex = xGetMemoryType(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(sVulkanDevice, &mai, nullptr, &texture->mMemory);
	vkBindImageMemory(sVulkanDevice, texture->mImage, texture->mMemory, 0);
}
void xSubmitImage2D(XTexture*texture, int width, int height, const void *pixel) {
	VkDeviceSize image_size = width * height;
	if (texture->mFormat==VK_FORMAT_R8G8B8A8_UNORM){
		image_size *= 4;
	}
	VkBuffer tempbuffer;
	VkDeviceMemory tempmemory;
	xGenBuffer(tempbuffer, tempmemory, image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	void*data;
	vkMapMemory(sVulkanDevice, tempmemory, 0, image_size, 0, &data);
	memcpy(data, pixel, image_size);
	vkUnmapMemory(sVulkanDevice, tempmemory);
	
	VkCommandBuffer commandbuffer;
	xBeginOneTimeCommandBuffer(&commandbuffer);
	VkImageSubresourceRange subresourcerange = {texture->mImageAspectFlag,0,1,0,1};
	xSetImageLayout(commandbuffer, texture->mImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourcerange);
	VkBufferImageCopy copy = {};
	copy.imageSubresource.aspectMask = texture->mImageAspectFlag;
	copy.imageSubresource.mipLevel = 0;
	copy.imageSubresource.baseArrayLayer = 0;
	copy.imageSubresource.layerCount = 1;

	copy.imageOffset = {0,0,0};
	copy.imageExtent = {uint32_t(width),uint32_t(height),1};
	vkCmdCopyBufferToImage(commandbuffer, tempbuffer, texture->mImage, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
	xSetImageLayout(commandbuffer, texture->mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,subresourcerange);
	xEndOneTimeCommandBuffer(commandbuffer);
	vkDestroyBuffer(sVulkanDevice, tempbuffer, nullptr);
	vkFreeMemory(sVulkanDevice, tempmemory,nullptr);
}
void xInitSrcAccessMask(VkImageLayout oldLayout, VkImageMemoryBarrier&barrier) {
	switch (oldLayout) {
	case VK_IMAGE_LAYOUT_UNDEFINED:
		barrier.srcAccessMask = 0;
		break;
	case VK_IMAGE_LAYOUT_PREINITIALIZED:
		barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		break;
	default:
		printf("init src access mask : unprocessed %d\n",oldLayout);
		break;
	}
}
void xInitDstAccessMask(VkImageLayout newLayout, VkImageMemoryBarrier&barrier) {
	switch (newLayout){
	case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		break;
	case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		break;
	case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | barrier.dstAccessMask;
		break;
	case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
		if (barrier.srcAccessMask==0){
			barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
		}
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		break;
	default:
		printf("init dst access mask : unprocessed %d\n",newLayout);
		break;
	}
}
void xSetImageLayout(VkCommandBuffer commandbuffer, VkImage image, VkImageLayout oldLayout,
	VkImageLayout newLayout, VkImageSubresourceRange subresourcerange,
	VkPipelineStageFlags src, VkPipelineStageFlags dst) {
	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.image = image;
	barrier.subresourceRange = subresourcerange;
	xInitSrcAccessMask(oldLayout, barrier);
	xInitDstAccessMask(newLayout, barrier);
	vkCmdPipelineBarrier(commandbuffer, src, dst, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}
void xGenImageView2D(XTexture*texture, int mipmap /* = 1 */) {
	VkImageViewCreateInfo ivci = {};
	ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ivci.image = texture->mImage;
	ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ivci.format = texture->mFormat;
	ivci.subresourceRange.aspectMask = texture->mImageAspectFlag;
	ivci.subresourceRange.baseMipLevel = 0;
	ivci.subresourceRange.levelCount = mipmap;
	ivci.subresourceRange.baseArrayLayer = 0;
	ivci.subresourceRange.layerCount = 1;
	vkCreateImageView(sVulkanDevice, &ivci, nullptr, &texture->mImageView);
}
void xGenSampler(XTexture*texture) {
	VkSamplerCreateInfo samplercreateinfo = {};
	samplercreateinfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplercreateinfo.minFilter = texture->mMinFilter;
	samplercreateinfo.magFilter = texture->mMagFilter;
	samplercreateinfo.addressModeU = texture->mWrapU;
	samplercreateinfo.addressModeV = texture->mWrapV;
	samplercreateinfo.addressModeW = texture->mWrapW;
	samplercreateinfo.anisotropyEnable = texture->mbEnableAnisotropy;
	samplercreateinfo.maxAnisotropy = texture->mMaxAnisotropy;
	samplercreateinfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	vkCreateSampler(sVulkanDevice, &samplercreateinfo, nullptr, &texture->mSampler);
}
void xInitDefaultTexture() {
	sDefaultTexture = new XTexture;
	sDefaultTexture->mFormat = VK_FORMAT_R8G8B8A8_UNORM;
	unsigned char *pixel = new unsigned char[256*256*4];
	memset(pixel, 255, sizeof(unsigned char)*256*256*4);
	xGenImage(sDefaultTexture, 256, 256, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		VK_IMAGE_USAGE_SAMPLED_BIT);
	xSubmitImage2D(sDefaultTexture, 256, 256, pixel);
	xGenImageView2D(sDefaultTexture);
	xGenSampler(sDefaultTexture);
	delete[] pixel;
}
void xConfigSampler2D(XProgram*program, int binding, VkImageView imageview, VkSampler sampler,
	VkImageLayout layout) {
	VkDescriptorSetLayoutBinding layoutbinding = {};
	layoutbinding.binding = binding;
	layoutbinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	layoutbinding.descriptorCount = 1;
	layoutbinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	layoutbinding.pImmutableSamplers = nullptr;
	program->mDescriptorSetLayoutBindings.push_back(layoutbinding);
	VkDescriptorPoolSize poolsize = {};
	poolsize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolsize.descriptorCount = 1;
	program->mDescriptorPoolSize.push_back(poolsize);
	VkDescriptorImageInfo *imageinfo = new VkDescriptorImageInfo;
	imageinfo->imageLayout = layout;
	imageinfo->imageView = imageview;
	imageinfo->sampler = sampler;
	VkWriteDescriptorSet wds = {};
	wds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	wds.dstSet = program->mDescriptorSet;
	wds.dstBinding = binding;
	wds.dstArrayElement = 0;
	wds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	wds.descriptorCount = 1;
	wds.pImageInfo = imageinfo;
	program->mWriteDescriptorSet.push_back(wds);
}
unsigned char * LoadImageFromFile(const char *path, int &width, int &height, int &channel,
	int force_channel, bool flipY) {
	unsigned char *result = stbi_load(path, &width, &height, &channel, force_channel);
	if (result==nullptr){
		return nullptr;
	}
	if (false==flipY){
		for (int j=0;j*2<height;++j){
			int index1 = j * width*channel;
			int index2 = (height - 1 - j)*width*channel;
			for (int i=width*channel;i>0;--i){
				unsigned char temp = result[index1];
				result[index1] = result[index2];
				result[index2] = temp;
				++index1;
				++index2;
			}
		}
	}
	return result;
}
XTexture *xGetDefaultTexture() {
	return sDefaultTexture;
}
void xRebindUniformBuffer(XProgram*program, int binding, XUniformBuffer*ubo) {
	VkDescriptorBufferInfo*bufferinfo = new VkDescriptorBufferInfo;
	bufferinfo->buffer = ubo->mBuffer;
	bufferinfo->offset = 0;
	if (ubo->mType==kXUniformBufferTypeMatrix){
		bufferinfo->range = sizeof(XMatrix4x4f)*ubo->mMatrices.size();
	}
	else {
		bufferinfo->range = sizeof(XVector4f)*ubo->mVector4s.size();
	}
	delete program->mWriteDescriptorSet[binding].pBufferInfo;
	program->mWriteDescriptorSet[binding].pBufferInfo = bufferinfo;
	vkUpdateDescriptorSets(sVulkanDevice, uint32_t(program->mWriteDescriptorSet.size()),
		program->mWriteDescriptorSet.data(), 0, nullptr);
}
void xRebindSampler(XProgram*program, int binding, VkImageView iv, VkSampler s,
	VkImageLayout layout) {
	VkDescriptorImageInfo*bufferinfo = new VkDescriptorImageInfo;
	bufferinfo->imageView = iv;
	bufferinfo->imageLayout = layout;
	bufferinfo->sampler = s;
	delete program->mWriteDescriptorSet[binding].pImageInfo;
	program->mWriteDescriptorSet[binding].pImageInfo = bufferinfo;
	vkUpdateDescriptorSets(sVulkanDevice, uint32_t(program->mWriteDescriptorSet.size()),
		program->mWriteDescriptorSet.data(), 0, nullptr);
}
VkCommandBuffer xBeginRendering(VkCommandBuffer commandbuffer) {
	VkCommandBuffer cmd;
	if (commandbuffer!=nullptr){
		cmd = commandbuffer;
	}
	else {
		xBeginOneTimeCommandBuffer(&cmd);
	}
	VkFramebuffer render_target = xAquireRenderTarget();
	VkRenderPass render_pass = sRenderPass;
	VkClearValue clearvalues[2] = {};
	clearvalues[0].color = {0.1f,0.4f,0.6f,1.0f};
	clearvalues[1].depthStencil = {1.0f,0};

	VkRenderPassBeginInfo rpbi = {};
	rpbi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpbi.framebuffer = render_target;
	rpbi.renderPass = render_pass;
	rpbi.renderArea.offset = {0,0};
	rpbi.renderArea.extent = {uint32_t(sViewportWidth),uint32_t(sViewportHeight)};
	rpbi.clearValueCount = 2;
	rpbi.pClearValues = clearvalues;
	vkCmdBeginRenderPass(cmd, &rpbi, VK_SUBPASS_CONTENTS_INLINE);
	sMainCommandBuffer = cmd;
	return cmd;
}
void xEndRendering() {
	vkCmdEndRenderPass(sMainCommandBuffer);
	vkEndCommandBuffer(sMainCommandBuffer);
}
static void xSubmitDrawCommand(VkCommandBuffer commandbuffer) {
	VkSemaphore ready_to_render[] = { sReadyToRender };
	VkSemaphore ready_to_present[] = {sReadyToPresent};
	VkSubmitInfo submitinfo = {};
	submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkPipelineStageFlags waitstages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitinfo.waitSemaphoreCount = 1;
	submitinfo.pWaitSemaphores = ready_to_render;
	submitinfo.pWaitDstStageMask = waitstages;
	submitinfo.pCommandBuffers = &commandbuffer;
	submitinfo.commandBufferCount = 1;
	submitinfo.signalSemaphoreCount = 1;
	submitinfo.pSignalSemaphores = ready_to_present;
	vkQueueSubmit(sGraphicQueue, 1, &submitinfo, VK_NULL_HANDLE);
}
static void PresentFrameBuffer() {
	VkSemaphore ready_to_present[] = {sReadyToPresent};
	VkPresentInfoKHR presentinfo = {};
	uint32_t current_render_target_index = sCurrentRenderFrameBufferIndex;
	presentinfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentinfo.pWaitSemaphores = ready_to_present;
	presentinfo.waitSemaphoreCount = 1;
	presentinfo.pSwapchains = &sSwapChain;
	presentinfo.swapchainCount = 1;
	presentinfo.pImageIndices = &current_render_target_index;
	vkQueuePresentKHR(sPresentQueue, &presentinfo);
	vkQueueWaitIdle(sPresentQueue);
}
void xSwapBuffers(VkCommandBuffer commandbuffer) {
	VkCommandBuffer cmd;
	if (commandbuffer==nullptr){
		cmd = sMainCommandBuffer;
	}
	else {
		cmd = commandbuffer;
	}
	xSubmitDrawCommand(cmd);
	PresentFrameBuffer();
	vkFreeCommandBuffers(sVulkanDevice, sCommandPool, 1, &cmd);
	sMainCommandBuffer = nullptr;
}
void xSetColorAttachmentCount(XFixedPipeline*pipeline, int count) {
	pipeline->mColorBlendAttachmentStates.resize(count);
	for (int i=0;i<count;++i){
		pipeline->mColorBlendAttachmentStates[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		pipeline->mColorBlendAttachmentStates[i].blendEnable = VK_FALSE;
		pipeline->mColorBlendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		pipeline->mColorBlendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		pipeline->mColorBlendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		pipeline->mColorBlendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		pipeline->mColorBlendAttachmentStates[i].colorBlendOp = VK_BLEND_OP_ADD;
		pipeline->mColorBlendAttachmentStates[i].alphaBlendOp = VK_BLEND_OP_ADD;
	}
}
void xEnableBlend(XFixedPipeline*pipeline, int attachment, VkBool32 enable) {
	pipeline->mColorBlendAttachmentStates[attachment].blendEnable = enable;
}
void xBlend(XFixedPipeline*p, int attachment, VkBlendFactor s_c, VkBlendFactor s_a,
	VkBlendFactor d_c, VkBlendFactor d_a) {
	p->mColorBlendAttachmentStates[attachment].srcColorBlendFactor = s_c;
	p->mColorBlendAttachmentStates[attachment].srcAlphaBlendFactor = s_a;
	p->mColorBlendAttachmentStates[attachment].dstColorBlendFactor = d_c;
	p->mColorBlendAttachmentStates[attachment].dstAlphaBlendFactor = d_a;
}
void xBlendOp(XFixedPipeline*p, int attachment, VkBlendOp color, VkBlendOp alpha) {
	p->mColorBlendAttachmentStates[attachment].colorBlendOp = color;
	p->mColorBlendAttachmentStates[attachment].alphaBlendOp = alpha;
}
void xPolygonMode(XFixedPipeline*p, VkPolygonMode mode) {
	p->mRasterizer.polygonMode = mode;
}
void xDisableRasterizer(XFixedPipeline*p, VkBool32 disable) {
	p->mRasterizer.rasterizerDiscardEnable = disable;
}
void xEnableDepthTest(XFixedPipeline*p, VkBool32 enable) {
	p->mDepthStencilState.depthTestEnable = enable;
}
void xInitPipelineLayout(XFixedPipeline*p) {
	VkPushConstantRange pushconstancrange = {};
	pushconstancrange.stageFlags = p->mPushConstantShaderStage;
	pushconstancrange.offset = 0;
	pushconstancrange.size = sizeof(XVector4f)*p->mPushConstantCount;
	VkPipelineLayoutCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	ci.pSetLayouts = p->mDescriptorSetLayout;
	ci.setLayoutCount = p->mDescriptorSetLayoutCount;
	ci.pPushConstantRanges = &pushconstancrange;
	ci.pushConstantRangeCount = 1;
	vkCreatePipelineLayout(sVulkanDevice, &ci, nullptr, &p->mPipelineLayout);
}
void xCreateFixedPipeline(XFixedPipeline*p) {
	const auto &bindingdescriptions = XVertexData::BindingDescription();
	const auto &attributeDescriptions = XVertexData::AttributeDescriptions();
	VkPipelineVertexInputStateCreateInfo vertexinputinfo = {};
	vertexinputinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexinputinfo.vertexBindingDescriptionCount = 1;
	vertexinputinfo.pVertexBindingDescriptions = &bindingdescriptions;
	vertexinputinfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexinputinfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	p->mColorBlendState.attachmentCount = p->mColorBlendAttachmentStates.size();
	p->mColorBlendState.pAttachments = p->mColorBlendAttachmentStates.data();
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH,
		VK_DYNAMIC_STATE_SCISSOR,
		VK_DYNAMIC_STATE_DEPTH_BIAS
	};
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 4;
	dynamicState.pDynamicStates = dynamicStates;
	VkGraphicsPipelineCreateInfo pipelineinfo = {};
	pipelineinfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineinfo.stageCount = p->mShaderStageCount;
	pipelineinfo.pStages = p->mShaderStages;
	pipelineinfo.pVertexInputState = &vertexinputinfo;
	pipelineinfo.pInputAssemblyState = &p->mInputAssetmlyState;
	pipelineinfo.pViewportState = &p->mViewportState;
	pipelineinfo.pRasterizationState = &p->mRasterizer;
	pipelineinfo.pMultisampleState = &p->mMultisampleState;
	pipelineinfo.pDepthStencilState = &p->mDepthStencilState;
	pipelineinfo.pColorBlendState = &p->mColorBlendState;
	pipelineinfo.pDynamicState = &dynamicState;
	pipelineinfo.layout = p->mPipelineLayout;
	pipelineinfo.renderPass = p->mRenderPass;
	pipelineinfo.subpass = 0;
	pipelineinfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineinfo.basePipelineIndex = -1;
	vkCreateGraphicsPipelines(sVulkanDevice, VK_NULL_HANDLE, 1, &pipelineinfo, nullptr,
		&p->mPipeline);
}
void xSetDynamicState(XFixedPipeline*p, VkCommandBuffer commandbuffer) {
	vkCmdSetViewport(commandbuffer, 0, 1, &p->mViewport);
	vkCmdSetScissor(commandbuffer, 0, 1, &p->mScissor);
	vkCmdSetDepthBias(commandbuffer, p->mDepthConstantFactor, p->mDepthClamp, p->mDepthSlopeFactor);
	vkCmdPushConstants(commandbuffer, p->mPipelineLayout, p->mPushConstantShaderStage, 0,
		sizeof(XVector4f)*p->mPushConstantCount, p->mPushConstants);
}
void xGenImageCube(XTexture*texture, uint32_t w, uint32_t h, VkFormat f,
	VkImageUsageFlags usage, VkSampleCountFlagBits sample_count, int mipmap) {
	VkImageCreateInfo ici = {};
	ici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ici.imageType = VK_IMAGE_TYPE_2D;
	ici.extent = { w,h,1 };
	ici.mipLevels = mipmap;
	ici.arrayLayers = 6;
	ici.format = f;
	ici.initialLayout = texture->mInitLayout;
	ici.usage = usage;
	ici.samples = sample_count;
	ici.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	if (vkCreateImage(sVulkanDevice, &ici, nullptr, &texture->mImage) != VK_SUCCESS) {
		printf("failed to create image\n");
	}
	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(sVulkanDevice, texture->mImage, &memory_requirements);
	VkMemoryAllocateInfo mai = {};
	mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mai.allocationSize = memory_requirements.size;
	mai.memoryTypeIndex = xGetMemoryType(memory_requirements.memoryTypeBits, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(sVulkanDevice, &mai, nullptr, &texture->mMemory);
	vkBindImageMemory(sVulkanDevice, texture->mImage, texture->mMemory, 0);
}
void xSubmitImageCube(XTexture*texture, int width, int height, const void *pixel) {
	VkDeviceSize offset_unit = width * height;
	if (texture->mFormat == VK_FORMAT_R8G8B8A8_UNORM) {
		offset_unit *= 4;
	}
	int imagesize = offset_unit * 6;
	VkBuffer tempbuffer;
	VkDeviceMemory tempmemory;
	xGenBuffer(tempbuffer, tempmemory, imagesize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	void*data;
	vkMapMemory(sVulkanDevice, tempmemory, 0, imagesize, 0, &data);
	memcpy(data, pixel, imagesize);
	vkUnmapMemory(sVulkanDevice, tempmemory);
	
	std::vector<VkBufferImageCopy> copies;
	for (uint32_t face=0;face<6;++face){
		VkBufferImageCopy copy = {};
		copy.imageSubresource.aspectMask = texture->mImageAspectFlag;
		copy.imageSubresource.mipLevel = 0;
		copy.imageSubresource.baseArrayLayer = face;
		copy.imageSubresource.layerCount = 1;

		copy.imageOffset = { 0,0,0 };
		copy.imageExtent = { uint32_t(width),uint32_t(height),1 };
		copy.bufferOffset = offset_unit*face;
		copies.push_back(copy);
	}
	VkCommandBuffer commandbuffer;
	xBeginOneTimeCommandBuffer(&commandbuffer);
	VkImageSubresourceRange subresourcerange = { texture->mImageAspectFlag,0,1,0,6 };
	xSetImageLayout(commandbuffer, texture->mImage, VK_IMAGE_LAYOUT_UNDEFINED, 
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourcerange);

	vkCmdCopyBufferToImage(commandbuffer, tempbuffer, texture->mImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6, copies.data());
	xSetImageLayout(commandbuffer, texture->mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourcerange);
	xEndOneTimeCommandBuffer(commandbuffer);

	vkDestroyBuffer(sVulkanDevice, tempbuffer, nullptr);
	vkFreeMemory(sVulkanDevice, tempmemory, nullptr);
}
void xGenImageViewCube(XTexture*texture, int mipmap /* = 1 */) {
	VkImageViewCreateInfo ivci = {};
	ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ivci.image = texture->mImage;
	ivci.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
	ivci.format = texture->mFormat;
	ivci.subresourceRange.aspectMask = texture->mImageAspectFlag;
	ivci.subresourceRange.baseMipLevel = 0;
	ivci.subresourceRange.levelCount = mipmap;
	ivci.subresourceRange.baseArrayLayer = 0;
	ivci.subresourceRange.layerCount = 6;
	ivci.components = {VK_COMPONENT_SWIZZLE_R,VK_COMPONENT_SWIZZLE_G,VK_COMPONENT_SWIZZLE_B,
		VK_COMPONENT_SWIZZLE_A};
	vkCreateImageView(sVulkanDevice, &ivci, nullptr, &texture->mImageView);
}
void xVulkanCleanUp() {
	if (sDefaultTexture!=nullptr){
		delete sDefaultTexture;
	}
	delete sColorBuffer;
	delete sDepthBuffer;
	delete[] sSystemFrameBuffer;
	for (int i = 0; i < sFrameBufferCount; ++i) {
		vkDestroyImageView(sVulkanDevice, sSwapChainImageViews[i], nullptr);
	}
	vkDestroySwapchainKHR(sVulkanDevice, sSwapChain, nullptr);
	vkDestroyRenderPass(sVulkanDevice, sRenderPass, nullptr);
	vkDestroySemaphore(sVulkanDevice, sReadyToRender, nullptr);
	vkDestroySemaphore(sVulkanDevice, sReadyToPresent, nullptr);
	vkDestroyCommandPool(sVulkanDevice, sCommandPool, nullptr);
	vkDestroyDevice(sVulkanDevice, nullptr);
}
static void xInitGlobalRenderPass() {
	VkSampleCountFlagBits sample_count = xGetGlobalFrameBufferSampleCount();
	VkAttachmentDescription colorattachment = {
		0,
		sSwapChainImageFormat,
		sample_count,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};
	if (sample_count==VK_SAMPLE_COUNT_1_BIT){
		colorattachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}
	VkAttachmentDescription depthAttachment = {
		0,
		VK_FORMAT_D24_UNORM_S8_UINT,
		sample_count,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};
	VkAttachmentDescription colorattachmentresolve = {
		0,
		sSwapChainImageFormat,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};
	VkAttachmentReference colorattachmentref = {0,VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentReference depthattachmentref = {1,VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
	VkAttachmentReference colorresolveref = {2,VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
	VkAttachmentDescription attachments[3];
	int attachmentcount = 2;
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorattachmentref;
	subpass.pDepthStencilAttachment = &depthattachmentref;
	if (sample_count==VK_SAMPLE_COUNT_1_BIT){
		memcpy(&attachments[0], &colorattachment, sizeof(VkAttachmentDescription));
		memcpy(&attachments[1], &depthAttachment, sizeof(VkAttachmentDescription));
	}
	else {
		subpass.pResolveAttachments = &colorresolveref;
		attachmentcount = 3;
		memcpy(&attachments[0], &colorattachment, sizeof(VkAttachmentDescription));
		memcpy(&attachments[1], &depthAttachment, sizeof(VkAttachmentDescription));
		memcpy(&attachments[2], &colorattachmentresolve, sizeof(VkAttachmentDescription));
	}
	VkRenderPassCreateInfo cpci = {};
	cpci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	cpci.attachmentCount = attachmentcount;
	cpci.pAttachments = attachments;
	cpci.subpassCount = 1;
	cpci.pSubpasses = &subpass;
	vkCreateRenderPass(sVulkanDevice, &cpci, nullptr, &sRenderPass);
}
static void xInitSystemColorBuffer() {
	sColorBuffer = new XTexture;
	sColorBuffer->mFormat = sSwapChainImageFormat;
	xGenImage(sColorBuffer, xGetViewportWidth(), xGetViewportHeight(), sColorBuffer->mFormat,
		VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		xGetGlobalFrameBufferSampleCount());
	xGenImageView2D(sColorBuffer);
}
static void xInitSystemDepthBuffer() {
	sDepthBuffer = new XTexture(VK_IMAGE_ASPECT_DEPTH_BIT);
	sDepthBuffer->mFormat = VK_FORMAT_D24_UNORM_S8_UINT;
	xGenImage(sDepthBuffer, xGetViewportWidth(), xGetViewportHeight(), sDepthBuffer->mFormat,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		xGetGlobalFrameBufferSampleCount());
	xGenImageView2D(sDepthBuffer);
}
static void xFrameBufferFinish(XSystemFrameBuffer*fbo) {
	VkImageView render_targets[3];
	int render_target_count = 2;
	if (fbo->mSampleCount!=VK_SAMPLE_COUNT_1_BIT){
		render_target_count = 3;
		render_targets[0] = fbo->mColorBuffer;
		render_targets[1] = fbo->mDepthBuffer;
		render_targets[2] = fbo->mResolveBuffer;
	}
	else {
		render_targets[0] = fbo->mResolveBuffer;
		render_targets[1] = fbo->mDepthBuffer;
	}
	VkFramebufferCreateInfo fbci = {};
	fbci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fbci.renderPass = sRenderPass;
	fbci.pAttachments = render_targets;
	fbci.attachmentCount = render_target_count;
	fbci.width = xGetViewportWidth();
	fbci.height = xGetViewportHeight();
	fbci.layers = 1;
	vkCreateFramebuffer(sVulkanDevice, &fbci, nullptr, &fbo->mFrameBuffer);
}
void xInitSystemFrameBuffer() {
	if (sRenderPass==0){
		xInitGlobalRenderPass();
	}
	xInitSystemColorBuffer();
	xInitSystemDepthBuffer();
	if (sSystemFrameBuffer==nullptr){
		sSystemFrameBuffer = new XSystemFrameBuffer[sFrameBufferCount];
	}
	VkSampleCountFlagBits sample_count = xGetGlobalFrameBufferSampleCount();
	int attachment_count = 2;
	if (sample_count!=VK_SAMPLE_COUNT_1_BIT){
		attachment_count = 3;
	}
	for (int i = 0; i < sFrameBufferCount;++i) {
		sSystemFrameBuffer[i].mSampleCount = sample_count;
		sSystemFrameBuffer[i].mDepthBuffer = sDepthBuffer->mImageView;
		sSystemFrameBuffer[i].mResolveBuffer = sSwapChainImageViews[i];
		if (sample_count!=VK_SAMPLE_COUNT_1_BIT){
			sSystemFrameBuffer[i].mColorBuffer = sColorBuffer->mImageView;
		}
		xFrameBufferFinish(&sSystemFrameBuffer[i]);
	}
}
XVulkanHandle xGetSystemFrameBuffer(int index) {
	return &sSystemFrameBuffer[index];
}
int xGetSystemFrameBufferCount() {
	return sFrameBufferCount;
}
VkRenderPass xGetGlobalRenderPass() {
	return sRenderPass;
}
void xViewport(int width, int height) {
	if (sViewportWidth!=width||sViewportHeight!=height){
		sViewportWidth = width;
		sViewportHeight = height;
		if (sDepthBuffer!=nullptr){
			delete sDepthBuffer;
		}
		if (sColorBuffer!=nullptr){
			delete sColorBuffer;
		}
		for (int i=0;i<sFrameBufferCount;++i){
			vkDestroyFramebuffer(sVulkanDevice, sSystemFrameBuffer[i].mFrameBuffer, nullptr);
			vkDestroyImageView(sVulkanDevice, sSwapChainImageViews[i], nullptr);
		}
		vkDestroySwapchainKHR(sVulkanDevice, sSwapChain, nullptr);
		xInitSwapChain();
		xInitSystemFrameBuffer();
	}
}
void xInitCommandPool() {
	VkCommandPoolCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	ci.queueFamilyIndex = sGraphicQueueFamily;
	vkCreateCommandPool(sVulkanDevice, &ci, nullptr, &sCommandPool);
}
VkCommandPool xGetCommandPool() {
	return sCommandPool;
}
void xInitSemaphores() {
	VkSemaphoreCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore(sVulkanDevice, &ci, nullptr, &sReadyToRender);
	vkCreateSemaphore(sVulkanDevice, &ci, nullptr, &sReadyToPresent);
}
VkSemaphore xGetReadyToRenderSemaphore() {
	return sReadyToRender;
}
VkSemaphore xGetReadyToPresentSemaphore() {
	return sReadyToPresent;
}
void xInitSwapChain() {
	uint32_t image_count = 2;//double buffer
	VkSwapchainCreateInfoKHR ci = {};
	ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	ci.surface = sVulkanSurface;
	ci.minImageCount = image_count;
	ci.imageFormat = sSwapChainImageFormat;
	ci.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	ci.imageExtent = {uint32_t(xGetViewportWidth()),uint32_t(xGetViewportHeight())};
	ci.imageArrayLayers = 1;
	ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	uint32_t queuefamilyindices[] = {uint32_t(sGraphicQueueFamily),uint32_t(sPresentQueueFamily)};
	if (sGraphicQueueFamily != sPresentQueueFamily){
		ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		ci.pQueueFamilyIndices = queuefamilyindices;
		ci.queueFamilyIndexCount = 2;
	}
	else {
		ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(sPhysicalDevice, sVulkanSurface, &capabilities);
	ci.preTransform = capabilities.currentTransform;
	ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	ci.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	ci.clipped = VK_TRUE;
	ci.oldSwapchain = VK_NULL_HANDLE;
	vkCreateSwapchainKHR(sVulkanDevice, &ci, nullptr, &sSwapChain);
	vkGetSwapchainImagesKHR(sVulkanDevice, sSwapChain, &image_count, nullptr);
	sSwapChainImages.resize(image_count);
	sFrameBufferCount = image_count;
	vkGetSwapchainImagesKHR(sVulkanDevice, sSwapChain, &image_count, sSwapChainImages.data());
	sSwapChainExtent = { uint32_t(xGetViewportWidth()),uint32_t(xGetViewportHeight())};
	sSwapChainImageViews.resize(image_count);
	for (int i=0;i<image_count;++i){
		VkImageViewCreateInfo ivci = {};
		ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ivci.image = sSwapChainImages[i];
		ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ivci.format = sSwapChainImageFormat;
		ivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ivci.subresourceRange.baseMipLevel = 0;
		ivci.subresourceRange.levelCount = 1;
		ivci.subresourceRange.baseArrayLayer = 0;
		ivci.subresourceRange.layerCount = 1;
		vkCreateImageView(sVulkanDevice, &ivci, nullptr, &sSwapChainImageViews[i]);
	}
}
VkSwapchainKHR xGetSwapChain() {
	return sSwapChain;
}
VkFormat xGetSwapChainImageFormat() {
	return sSwapChainImageFormat;
}
void xInitVulkanDevice() {
	VkDeviceQueueCreateInfo queue_create_info[2];
	int queue_count = 2;
	float priority = 1.0f;
	queue_create_info[0] = {};
	queue_create_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info[0].queueCount = 1;
	queue_create_info[0].queueFamilyIndex = sGraphicQueueFamily;
	queue_create_info[0].pQueuePriorities = &priority;
	queue_create_info[1] = {};
	queue_create_info[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info[1].queueCount = 1;
	queue_create_info[1].queueFamilyIndex = sPresentQueueFamily;
	queue_create_info[1].pQueuePriorities = &priority;
	if (sGraphicQueueFamily == sPresentQueueFamily){
		queue_count = 1;
	}
	const char * extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkPhysicalDeviceFeatures features = {};
	features.samplerAnisotropy = VK_TRUE;
	features.geometryShader = VK_TRUE;
	features.fillModeNonSolid = VK_TRUE;
	features.sampleRateShading = VK_TRUE;

	const char * layers[] = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	VkDeviceCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	ci.pQueueCreateInfos = queue_create_info;
	ci.queueCreateInfoCount = queue_count;
	ci.enabledExtensionCount = 1;
	ci.ppEnabledExtensionNames = extensions;
	ci.enabledLayerCount = 1;
	ci.ppEnabledLayerNames = layers;
	ci.pEnabledFeatures = &features;
	vkCreateDevice(sPhysicalDevice, &ci, nullptr, &sVulkanDevice);
	vkGetDeviceQueue(sVulkanDevice, sGraphicQueueFamily, 0, &sGraphicQueue);
	vkGetDeviceQueue(sVulkanDevice, sPresentQueueFamily, 0, &sPresentQueue);
}
VkDevice xGetVulkanDevice() {
	return sVulkanDevice;
}
VkQueue xGetGraphicQueue() {
	return sGraphicQueue;
}
VkQueue xGetPresentQueue() {
	return sPresentQueue;
}
int xGetGraphicQueueFamily() {
	return sGraphicQueueFamily;
}
int xGetPresentQueueFamily() {
	return sPresentQueueFamily;
}
VkPhysicalDevice xGetVulkanPhysicalDevice() {
	return sPhysicalDevice;
}
VkSampleCountFlagBits xGetMaxMSAASampleCount() {
	VkPhysicalDeviceProperties physicalproperties;
	vkGetPhysicalDeviceProperties(sPhysicalDevice, &physicalproperties);
	VkSampleCountFlags count = physicalproperties.limits.framebufferColorSampleCounts >
		physicalproperties.limits.framebufferDepthSampleCounts ?
		physicalproperties.limits.framebufferDepthSampleCounts :
		physicalproperties.limits.framebufferColorSampleCounts;
	if (count&VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (count&VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (count&VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (count&VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (count&VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (count&VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }
	return VK_SAMPLE_COUNT_1_BIT;
}
void xInitVulkanPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(sVulkanInstance, &deviceCount, nullptr);
	VkPhysicalDevice*devices = new VkPhysicalDevice[deviceCount];
	vkEnumeratePhysicalDevices(sVulkanInstance, &deviceCount, devices);
	for (uint32_t i=0;i<deviceCount;++i){
		VkPhysicalDevice *current_device = &devices[i];
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceProperties(*current_device, &properties);
		vkGetPhysicalDeviceFeatures(*current_device, &features);
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(*current_device, &queue_family_count, nullptr);
		VkQueueFamilyProperties *queuefamilyproperties = new VkQueueFamilyProperties[queue_family_count];
		vkGetPhysicalDeviceQueueFamilyProperties(*current_device, &queue_family_count, queuefamilyproperties);
		sPresentQueueFamily = -1;
		sGraphicQueueFamily = -1;
		for (uint32_t j=0;j<queue_family_count;++j){
			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(*current_device, j, sVulkanSurface, &present_support);
			if (queuefamilyproperties[j].queueCount > 0 && present_support) {
				sPresentQueueFamily = j;
			}
			if (queuefamilyproperties[j].queueCount>0&&
				queuefamilyproperties[j].queueFlags&VK_QUEUE_GRAPHICS_BIT){
				sGraphicQueueFamily = j;
			}
			if (sGraphicQueueFamily!=-1&&sPresentQueueFamily!=-1){
				sPhysicalDevice = *current_device;
				sMaxSampleCount = xGetMaxMSAASampleCount();
				return;
			}
		}
		delete[] queuefamilyproperties;
	}
	delete[]devices;
}
static void InitVulkanInstance() {
	VkApplicationInfo appinfo = {};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pApplicationName = "BattleFireEngine";
	appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appinfo.pEngineName = "Alice";
	appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appinfo.apiVersion = VK_API_VERSION_1_1;

	const char * extensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};
	const char * layers[] = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	VkInstanceCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	ci.pApplicationInfo = &appinfo;
	ci.enabledExtensionCount = 3;
	ci.ppEnabledExtensionNames = extensions;
	ci.enabledLayerCount = 1;
	ci.ppEnabledLayerNames = layers;
	vkCreateInstance(&ci, nullptr, &sVulkanInstance);
}
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage,
	void*                                       pUserData
) {
	printf("validation layer : %s\n",pMessage);
	return VK_FALSE;
}
static void InitDebugger() {
	VkDebugReportCallbackCreateInfoEXT ci = {};
	ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	ci.pfnCallback = debug_callback;
	__vkCreateDebugReportCallback(sVulkanInstance, &ci, nullptr, &sVulkanDebugger);
}
static void InitSurface() {
	VkWin32SurfaceCreateInfoKHR ci = {};
	ci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	ci.hinstance = GetModuleHandle(NULL);
	ci.hwnd = (HWND)sWindowHWND;
	__vkCreateWin32SurfaceKHR(sVulkanInstance, &ci, nullptr, &sVulkanSurface);
}
void xInitVulkan(void*param, int width, int height) {
	sWindowHWND = param;
	sViewportWidth = width;
	sViewportHeight = height;
	InitVulkanInstance();
	__vkCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
		sVulkanInstance,
		"vkCreateDebugReportCallbackEXT");
	__vkDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
		sVulkanInstance,
		"vkDestroyDebugReportCallback");
	__vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(
		sVulkanInstance,
		"vkCreateWin32SurfaceKHR");
	InitDebugger();
	InitSurface();
	xInitVulkanPhysicalDevice();
	xInitVulkanDevice();
	xInitSwapChain();
	xInitSystemFrameBuffer();
	xInitCommandPool();
	xInitSemaphores();
	xInitDefaultTexture();
}
VkSampleCountFlagBits xGetGlobalFrameBufferSampleCount() {
#if MSAA
	return sMaxSampleCount;
#else
	return VK_SAMPLE_COUNT_1_BIT;
#endif
}
int xGetViewportWidth() {
	return sViewportWidth;
}
int xGetViewportHeight() {
	return sViewportHeight;
}
VkFramebuffer xAquireRenderTarget() {
	vkAcquireNextImageKHR(sVulkanDevice, sSwapChain, 1000000000, sReadyToRender, VK_NULL_HANDLE,
		&sCurrentRenderFrameBufferIndex);
	return sSystemFrameBuffer[sCurrentRenderFrameBufferIndex].mFrameBuffer;
}
uint32_t xGetCurrenRenderTargetIndex() {
	return sCurrentRenderFrameBufferIndex;
}