#include "Skybox.h"
#include "Runtime/Render/AliceGL.h"
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/Camera.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Render/RenderState.h"
#include "Runtime/Render/RenderPass.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Render/Material.h"

namespace Alice{
	SkyBox::SkyBox(){
		mVertexData = new VertexData;
		mVertexData->SetBufferSize(36);
		GLfloat skyboxVertices[] = {
			// Positions
			//front
			-0.5f,  -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,

			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f
		};
		float *data = skyboxVertices;
		for (int i=0;i<36;++i){
			mVertexData->GetBuffer<VertexDataFull>()[i].mVertex.Set(data[0], data[1], data[2], 1.0f);
			data += 3;
		}
		mVBO.Init(36);
		VertexDataFull*vertexes = mVertexData->GetBuffer<VertexDataFull>();
		mVBO.SubData(vertexes, mVertexData->mBufferSize);
	}

	void SkyBox::Render(){
		if (mMaterial!=nullptr){
			RenderPass*pass = mMaterial->GetPass(0);
			if (pass != nullptr) {
				Camera*camera = (Camera*)mCamera;
				const Matrix4x4 & world_matrix = camera->mOwner->GetWorldMatrix();
				float modelMatrix[] = {
					1.0f,0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f,0.0f,
					0.0f,0.0f,1.0f,0.0f,
					world_matrix.mData[12], world_matrix.mData[13], world_matrix.mData[14],1.0f
				};
				pass->SetMatrix4Property(UniformDefines::Matrix_M_NameInShader, modelMatrix);
				mVBO.Active();
				pass->ActiveRenderState();
				pass->UpdateUniforms(camera);
				pass->SetupStencilBuffer();
				mVBO.Draw(GL_TRIANGLES);
				pass->RestoreStencilBuffer();
			}
		}
	}
}