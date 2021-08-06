#include "ground.h"
void Patch::Init(float xOffset, float zOffset) {
	mCurrentLODLevel = 0;
	//mVertexBuffer = new VertexBuffer;
	//mVertexBuffer->SetSize(25);
	mIBO = 0;
	mPosition.x = xOffset;
	mPosition.y = 0.0f;
	mPosition.z = zOffset;
	//for (int z = 0; z < 5; ++z) {
	//	float zPosition = zOffset + 2.0f - z;
	//	for (int x = 0; x < 5; ++x) {
	//		float xPosition = -2.0f + x + xOffset;
	//		int point_index = x + z * 5;
	//		mVertexBuffer->SetPosition(point_index, xPosition, 0.0f, zPosition);
	//		mVertexBuffer->SetNormal(point_index, 0.0f, 1.0f, 0.0f);
	//	}
	//}
}
void Patch::Draw(Shader* shader) {
	mVertexBuffer->Bind();
	glEnableVertexAttribArray(shader->mPositionLocation);
	glVertexAttribPointer(shader->mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(shader->mColorLocation);
	glVertexAttribPointer(shader->mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));
	glEnableVertexAttribArray(shader->mTexcoordLocation);
	glVertexAttribPointer(shader->mTexcoordLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));
	glEnableVertexAttribArray(shader->mNormalLocation);
	glVertexAttribPointer(shader->mNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glDrawElements(GL_TRIANGLES,
		mCurrentLODLevelIndexCount,
		GL_UNSIGNED_SHORT, 
		0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	mVertexBuffer->Unbind();
}
void Ground::Update(glm::vec3 camera_pos) {
	for (int z = 0; z < 20; ++z) {
		for (int x = 0; x < 20; ++x) {
			int patch_index = (x + z * 20);
			float distance_from_camera=glm::distance(mPatches[patch_index].mPosition,camera_pos);
			if (distance_from_camera < 20.0f) {
				mPatches[patch_index].mCurrentLODLevel = 0;
			}
			else if (distance_from_camera < 100.0f) {
				mPatches[patch_index].mCurrentLODLevel = 1;
			}
			else if (distance_from_camera < 200.0f) {
				mPatches[patch_index].mCurrentLODLevel = 2;
			}
		}
	}
	//calculate patch index buffer
	for (int z = 0; z < 20; ++z) {
		for (int x = 0; x < 20; ++x) {
			int patch_index = (x + z * 20);
			mPatches[patch_index].UpdateIndexBuffer();
		}
	}
}
void Patch::UpdateIndexBuffer() {
	if (mCurrentLODLevel == 0) {
		unsigned short indices[96] = { 0 };
		int index_offset = 0;
		for (int z = 0; z < 4; ++z) {
			for (int x = 0; x < 4; ++x) {
				int lb_index = x + z * 5;
				int rb_index = x + 1 + z * 5;
				int lt_index = x + 5 * (z + 1);
				int rt_index = x + 1 + 5 * (z + 1);
				indices[index_offset++] = lb_index;
				indices[index_offset++] = rb_index;
				indices[index_offset++] = lt_index;

				indices[index_offset++] = lt_index;
				indices[index_offset++] = rb_index;
				indices[index_offset++] = rt_index;
			}
		}
		if (mIBO!=0){
			glDeleteBuffers(1, &mIBO);
		}
		mCurrentLODLevelIndexCount = 96;
		mIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), GL_STATIC_DRAW, indices);
	}
	else if (mCurrentLODLevel == 1) {
		unsigned short indices[24] = { 0 };
		int index_offset = 0;
		for (int z = 0; z < 2; ++z) {
			for (int x = 0; x < 2; ++x) {
				int lb_index = x * 2 + 2 * z * 5;
				int rb_index = (x + 1) * 2 + z * 5 * 2;
				int lt_index = x * 2 + 5 * (z + 1) * 2;
				int rt_index = (x + 1) * 2 + 5 * (z + 1) * 2;

				indices[index_offset++] = lb_index;
				indices[index_offset++] = rb_index;
				indices[index_offset++] = lt_index;

				indices[index_offset++] = lt_index;
				indices[index_offset++] = rb_index;
				indices[index_offset++] = rt_index;
			}
		}
		if (mIBO != 0) {
			glDeleteBuffers(1, &mIBO);
		}
		mCurrentLODLevelIndexCount = 24;
		mIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), GL_STATIC_DRAW, indices);
	}
	else if (mCurrentLODLevel == 2) {
		unsigned short indices[6] = { 0 };
		int index_offset = 0;
		indices[index_offset++] = 0;
		indices[index_offset++] = 4;
		indices[index_offset++] = 20;

		indices[index_offset++] = 20;
		indices[index_offset++] = 4;
		indices[index_offset++] = 24; 
		if (mIBO != 0) {
			glDeleteBuffers(1, &mIBO);
		}
		mCurrentLODLevelIndexCount = 6;
		mIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), GL_STATIC_DRAW, indices);
	}
}
void Ground::Init() {
	mVertexBuffer = new VertexBuffer;
	mVertexBuffer->SetSize(6561);
	mPatches = new Patch[400];
	for (int zPatchIndex = 0; zPatchIndex < 20; ++zPatchIndex) {
		float zCenterPosition = 40.0f - 2.0f - zPatchIndex * 4.0f;
		for (int xPatchIndex = 0; xPatchIndex < 20; ++xPatchIndex) {
			float xCenterPosition = xPatchIndex * 4.0f - 40.0f + 2.0f;
			int patch_index = (xPatchIndex + zPatchIndex * 20);
			mPatches[patch_index].Init(xCenterPosition,zCenterPosition);

			int xDirectionPatchPointStartIndex = 1;
			if (xPatchIndex==0){//5 points
				xDirectionPatchPointStartIndex = 0;
			}
			int zDirectionPatchPointStartIndex = 1;
			if (zPatchIndex == 0) {
				zDirectionPatchPointStartIndex = 0;
			}
			//generate patch points
			for (int z = zDirectionPatchPointStartIndex; z < 5; ++z) {
				float zPosition = zCenterPosition + 2.0f - z;
				for (int x = xDirectionPatchPointStartIndex; x < 5; ++x) {
					float xPosition = -2.0f + x + xCenterPosition;
					int xPatchPointIndexInTerrainTiled = x + xPatchIndex * 4;
					int zPatchPointIndexInTerrainTiled = z + zPatchIndex * 4;
					//zPatchIndex = 0,xPatchIndex = 0 , x -> 0~4 
					//xPatchPointIndexInTerrainTiled -> 0~4
					//zPatchIndex = 0,xPatchIndex = 1 , x -> 1~4 
					//xPatchPointIndexInTerrainTiled -> 4 + 1~4 : 5 -> 8
					//zPatchIndex = 0,xPatchIndex = 19 , x -> 1~4 
					//xPatchPointIndexInTerrainTiled -> 19*4(76) + 1~4 : 77 -> 80
					int point_index = xPatchPointIndexInTerrainTiled + zPatchPointIndexInTerrainTiled * 81;
					mVertexBuffer->SetPosition(point_index, xPosition, 0.0f, zPosition);
					mVertexBuffer->SetNormal(point_index, 0.0f, 1.0f, 0.0f);
				}
			}
		}
	}
	mShader = new Shader;
	mShader->Init("Res/ground.vs", "Res/ground.fs");
	mShader->SetVec4("U_LightPos", 0.0f, 0.0f, 1.0f, 0.0f);
	mShader->SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
	mShader->SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
	mShader->SetVec4("U_LightOpt", 32.0f, 0.0f, 0.0f, 1.0f);
	mShader->SetVec4("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
	mShader->SetVec4("U_DiffuseMaterial", 0.1f, 0.4f, 0.6f, 1.0f);
}
void Ground::Draw(glm::mat4 & viewMatrix, glm::mat4 & projectionMatrix) {
	glEnable(GL_DEPTH_TEST);
	mVertexBuffer->Bind();
	mShader->Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
	for (int z = 0; z < 20; ++z) {
		for (int x = 0; x < 20; ++x) {
			int patch_index = (x + z * 20);
			//mPatches[patch_index].Draw(mShader);
		}
	}
	mVertexBuffer->Unbind();
}