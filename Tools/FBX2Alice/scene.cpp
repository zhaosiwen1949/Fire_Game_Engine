#include "scene.h"
#include "XVulkan.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "Texture2D.h"
#include "Material.h"
#include "FSQ.h"
#include "Ground.h"
#include "Model.h"
#include "FrameBuffer.h"
#include <thread>
#include "fbx.h"
#include "fbxgpudraw.h"
#include "fbxskeletongpu.h"
Alice::FBXSkeletonGPU fbx;
Material *material = nullptr;
XFixedPipeline*pipeline = nullptr;
void Init() {
	glm::vec3 camera_pos(0.0f, 0.0f, 10.0f);
	material = new Material;
	material->Init("Res/skeleton.vsb", "Res/skeleton.fsb");
	glm::mat4 model=glm::translate(0.0f,-5.0f,-8.0f)*glm::scale(0.1f,0.1f,0.1f)*glm::rotate(-90.0f,0.0f,1.0f,0.0f);
	glm::mat4 projection = glm::perspective(45.0f, float(xGetViewportWidth()) / float(xGetViewportHeight()), 0.1f, 100.0f);
	projection[1][1] *= -1.0f;
	glm::mat4 view = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	material->SetMVP(model, view, projection);

	pipeline = new XFixedPipeline;
	xSetColorAttachmentCount(pipeline, 1);
	pipeline->mRenderPass = xGetGlobalRenderPass();
	pipeline->mViewport = { 0.0f,0.0f,float(xGetViewportWidth()),float(xGetViewportHeight()),0.0f,1.0f };
	pipeline->mScissor = { {0,0},{uint32_t(xGetViewportWidth()),uint32_t(xGetViewportHeight())} };
	pipeline->mInputAssetmlyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	xPolygonMode(pipeline, VK_POLYGON_MODE_LINE);
	material->SetFixedPipeline(pipeline);
	material->Finish();

	fbx.Init("tauren.fbx");
	fbx.InitForGPURendering();
	fbx.mMaterial = material;

	int joint_count = fbx.mTPoseJoints.size();
	for (int joint_index=0;joint_index!= joint_count;++joint_index){
		Alice::Joint*joint = fbx.mTPoseJoints[joint_index];
		material->mVertexMat4UBO->SetMatrix(4 + joint_index, joint->mTPoseInverseMatrix);
		material->mVertexMat4UBO->SetMatrix(260 + joint_index, joint->mGlobalMatrix);
	}
	material->SubmitUniformBuffers();
}
void Draw(float deltaTime) {
	fbx.Update(deltaTime);
	int joint_count = fbx.mTPoseJoints.size();
	for (int joint_index = 0; joint_index < joint_count; ++joint_index) {
		material->mVertexMat4UBO->SetMatrix(260 + joint_index, fbx.mCurrentAnimateJoints[joint_index].mGlobalTransform);
	}
	material->SubmitUniformBuffers();
	VkCommandBuffer commandbuffer = xBeginRendering();
	fbx.DrawGPUVersion(commandbuffer);
	xEndRendering();
	xSwapBuffers();
}
void OnViewportChanged(int width, int height) {
	xViewport(width, height);
	pipeline->mViewport = { 0.0f,0.0f,float(xGetViewportWidth()),float(xGetViewportHeight()),0.0f,1.0f };
	pipeline->mScissor = { {0,0},{uint32_t(xGetViewportWidth()),uint32_t(xGetViewportHeight())} };
}
void OnQuit() {
	if (pipeline != nullptr) {
		delete pipeline;
	}
	delete fbx.mMeshes[0]->mVertexBuffer;
	delete fbx.mMeshes[0]->mIndexBuffer;
	Material::CleanUp();
	xVulkanCleanUp();
}