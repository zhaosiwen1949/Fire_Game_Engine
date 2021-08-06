#include "scene.h"
#include "utils.h"
#include "shader.h"
#include "framebufferobject.h"
#include "RenderState.h"
#include "Material.h"
#include "GL20/VBO.h"
#include "DrawCall.h"
#include "Light.h"
#include "model.h"
struct Rect {
	float mLeft, mBottom, mWidth, mHeight;
	Rect(float l, float b, float w, float h) {
		mLeft = l;
		mBottom = b;
		mWidth = w;
		mHeight = h;
	}
};
Material*deffered_position_material = nullptr,*deffered_normal_material=nullptr,* deffered_material_color=nullptr;
VBO *vbo = nullptr;
ScreenQuad ** quads=nullptr;
Shader* fsqshader = nullptr;
glm::mat4 m,m2;
float color[4] = {0.74f,0.78f,0.68f,1.0f};
Camera camera;
DrawCall dc;
FrameBufferObject* fbo,*positionfbo,*normalfbo,*colorfbo;
float sViewportWidth = 0.0f, sViewportHeight = 0.0f;
Light* black_light = new Light;
GLUquadric* point_light_geometry = nullptr,*point_light_disk=nullptr;
int sScreenSplit = 32;
GLuint sSplitFrustumProgram=0;
GLuint sFrustumSSBO = 0;
struct FrustumVolume {
	glm::vec4 CommonPlanes[4];//a,b,c,d
	glm::vec4 NearFarPlanes[32];
};
ScreenQuad*InitQuad(float offsetx,float offsety,float size,int split_count_z =1) {
	VertexData vertices[4];
	vertices[0].Position[0] = offsetx;
	vertices[0].Position[1] = offsety;
	vertices[0].Position[2] = 0.0f;
	vertices[0].Position[3] = 1.0f;
	vertices[0].Texcoord[0] = (vertices[0].Position[0] - (-1.0f)) / 2.0f;
	vertices[0].Texcoord[1] = (vertices[0].Position[1] - (-1.0f)) / 2.0f;
	vertices[0].Texcoord[2] = 0.0f;
	vertices[0].Texcoord[3] = 0.0f;

	vertices[1].Position[0] = offsetx+size;
	vertices[1].Position[1] = offsety;
	vertices[1].Position[2] = 0.0f;
	vertices[1].Position[3] = 1.0f;
	vertices[1].Texcoord[0] = (vertices[1].Position[0]-(-1.0f))/2.0f;
	vertices[1].Texcoord[1] = (vertices[1].Position[1] - (-1.0f)) / 2.0f;
	vertices[1].Texcoord[2] = 0.0f;
	vertices[1].Texcoord[3] = 0.0f;

	vertices[2].Position[0] = offsetx;
	vertices[2].Position[1] = offsety + size;
	vertices[2].Position[2] = 0.0f;
	vertices[2].Position[3] = 1.0f;
	vertices[2].Texcoord[0] = (vertices[2].Position[0] - (-1.0f)) / 2.0f;
	vertices[2].Texcoord[1] = (vertices[2].Position[1] - (-1.0f)) / 2.0f;
	vertices[2].Texcoord[2] = 0.0f;
	vertices[2].Texcoord[3] = 0.0f;

	vertices[3].Position[0] = offsetx + size;
	vertices[3].Position[1] = offsety + size;
	vertices[3].Position[2] = 0.0f;
	vertices[3].Position[3] = 1.0f;
	vertices[3].Texcoord[0] = (vertices[3].Position[0] - (-1.0f)) / 2.0f;
	vertices[3].Texcoord[1] = (vertices[3].Position[1] - (-1.0f)) / 2.0f;
	vertices[3].Texcoord[2] = 0.0f;
	vertices[3].Texcoord[3] = 0.0f;

	ScreenQuad*quad = new ScreenQuad;
	quad->mVBO = new VBO;
	quad->mVBO->SetSize(4);
	quad->mVBO->SubmitData(vertices, sizeof(VertexData) * 4);
	/*float quad_left = offsetx;
	float quad_bottom = offsety;
	float quad_width = size;
	float quad_height = size;
	quad->mCommonPlanes[Frustum::kFrustumPlaneLeft].SetABCD(1.0f, 0.0f, 0.0f, -quad_left);
	quad->mCommonPlanes[Frustum::kFrustumPlaneRight].SetABCD(-1.0f, 0.0f, 0.0f, quad_left + quad_width);
	quad->mCommonPlanes[Frustum::kFrustumPlaneBottom].SetABCD(0.0f, 1.0f, 0.0f, -quad_bottom);
	quad->mCommonPlanes[Frustum::kFrustumPlaneTop].SetABCD(0.0f, -1.0f, 0.0f, quad_bottom + quad_height);
	float step = 2.0f / split_count_z;
	for (int i = 0; i < split_count_z; ++i) {
		Frustum* frustum = new Frustum;
		float z_start = i * step;
		float z_end = i * step + step;
		NearFarPlanePair* nf = new NearFarPlanePair;
		nf->mNearPlane.SetABCD(0.0f, 0.0f, 1.0f, 1.0f - z_start);
		nf->mFarPlane.SetABCD(0.0f, 0.0f, -1.0f, -(1.0f-z_end));
		quad->mZSplitPlanes.push_back(nf);
	}*/
	return quad;
}
bool IsOverlappedWith(const Rect& r1,const Rect &r2) {
	if (r1.mLeft > (r2.mLeft + r2.mWidth) || r2.mLeft > (r1.mLeft + r1.mWidth))
		return false;
	if ((r1.mBottom + r1.mHeight) < r2.mBottom || (r2.mBottom + r2.mHeight) < r1.mBottom)
		return false;
	return true;
}
void Init() {
	point_light_geometry = gluNewQuadric();
	point_light_disk = gluNewQuadric();
	GloblalRenderState::Init();
	Model model;
	model.Init("Res/Sphere.obj");
	vbo = new VBO;
	vbo->SetSize(model.mVertexCount);
	vbo->SubmitData(model.mData, sizeof(VertexData) * model.mVertexCount);

	Light* light = new Light;
	Light::mMainLight = light;
	light->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	

	Light* point_light = new Light;
	point_light->mType = kLightTypePoint;
	point_light->SetDiffuseColor(0.1f, 0.4f, 0.7f, 1.0f);
	point_light->SetSetting1(1.5f, 0.0f, 0.0f, 0.0f);
	point_light->SetPosition(-1.3f, 0.0f, -3.0f, 1.0f);
	Light::mLights.insert(point_light);

	point_light = new Light;
	point_light->mType = kLightTypePoint;
	point_light->SetDiffuseColor(0.1f, 0.4f, 0.7f, 1.0f);
	point_light->SetSetting1(1.5f, 0.0f, 0.0f, 0.0f);
	point_light->SetPosition(1.3f, 0.0f, -3.0f, 1.0f);
	Light::mLights.insert(point_light);

	black_light->mType = kLightTypePoint;
	black_light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	m = glm::translate(0.0f, 0.0f, -3.0f);

	camera.mViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));

	deffered_position_material = new Material;
	deffered_position_material->mBaseRenderPass = new RenderPass;
	deffered_position_material->mBaseRenderPass->SetShader(Shader::LoadShader("Res/DefferedPosition", "Res/DefferedPosition.vs", "Res/DefferedPosition.fs"));
	deffered_position_material->mBaseRenderPass->SetMatrix4("ModelMatrix", glm::value_ptr(m));

	deffered_normal_material = new Material;
	deffered_normal_material->mBaseRenderPass = new RenderPass;
	deffered_normal_material->mBaseRenderPass->SetShader(Shader::LoadShader("Res/DefferedNormal", "Res/DefferedNormal.vs", "Res/DefferedNormal.fs"));
	deffered_normal_material->mBaseRenderPass->SetMatrix4("ModelMatrix", glm::value_ptr(m));

	deffered_material_color = new Material;
	deffered_material_color->mBaseRenderPass = new RenderPass;
	deffered_material_color->mBaseRenderPass->SetShader(Shader::LoadShader("Res/DefferedColor", "Res/DefferedColor.vs", "Res/DefferedColor.fs"));
	deffered_material_color->mBaseRenderPass->SetMatrix4("ModelMatrix", glm::value_ptr(m));
	GLuint texture = CreateTexture2DFromBMP("res/test.bmp");
	deffered_material_color->mBaseRenderPass->SetSampler2D("U_Texture",texture);

	dc.mVBO = vbo;

	fsqshader = new Shader;
	fsqshader->Init("Res/fullscreenquad.vs", "Res/texture.fs");
}
glm::vec4 ProjectPointToScreenSpace(Camera &camera,glm::vec4 point_to_project) {
	glm::vec4 clip_space_pos = camera.mProjectionMatrix * point_to_project;
	glm::vec4 ndc_space_pos = glm::vec4(
		clip_space_pos.x / clip_space_pos.w, clip_space_pos.y / clip_space_pos.w,
		clip_space_pos.z / clip_space_pos.w, clip_space_pos.w);
	glm::vec4 projected_point(
		ndc_space_pos.x * sViewportWidth / 2.0,
		ndc_space_pos.y * sViewportHeight / 2.0f,
		0.0f,
		1.0f);
	return projected_point;
}
void CaculateScreenSpaceLightGeometry(Light*light,glm::vec4&projected_position,float & projected_radius) {
	glm::vec4 point_light_world_pos(light->mPosition[0],light->mPosition[1],light->mPosition[2],light->mPosition[3]);
	glm::vec4 point_light_view_space_pos = camera.mViewMatrix * point_light_world_pos;
	glm::vec4 point_on_sphere = point_light_view_space_pos + glm::vec4(light->mSetting1[0], 0.0f, 0.0f, 0.0f);
	projected_position = ProjectPointToScreenSpace(camera, point_light_view_space_pos);
	glm::vec4 temp = ProjectPointToScreenSpace(camera, point_on_sphere);
	projected_radius = temp.x - projected_position.x;
}
void SplitFrusumViaComputeShader(const float * world_to_clip) {
	printf("cpu matrix :\nM4:%f,%f,%f,%f\nM3:%f,%f,%f,%f\nM2:%f,%f,%f,%f\nM1:%f,%f,%f,%f\n",
		world_to_clip[3], world_to_clip[7], world_to_clip[11], world_to_clip[15],
		world_to_clip[2], world_to_clip[6], world_to_clip[10], world_to_clip[14],
		world_to_clip[1], world_to_clip[5], world_to_clip[9], world_to_clip[13],
		world_to_clip[0], world_to_clip[4], world_to_clip[8], world_to_clip[12]);
	sSplitFrustumProgram = CreateComputeProgram("Res/SplitFrusum.compute");
	glGenBuffers(1, &sFrustumSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, sFrustumSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*4*36*32*32, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(sSplitFrustumProgram);
	glUniformMatrix4fv(
		glGetUniformLocation(sSplitFrustumProgram,"U_World_To_Clip"),
		1,
		GL_TRUE,
		world_to_clip);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sFrustumSSBO);
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glUseProgram(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	if (glGetError() != GL_NO_ERROR) {
		printf("xxxxx\n");
	}
}
void SetViewPortSize(float width, float height) {
	sViewportWidth = width;
	sViewportHeight = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, width / height, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.mProjectionMatrix = glm::perspective(45.0f, width / height, 0.1f, 1000.0f);
	glm::mat4 world_to_clip_space = camera.mViewMatrix * camera.mProjectionMatrix;
	SplitFrusumViaComputeShader(glm::value_ptr(world_to_clip_space));
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, sFrustumSSBO);
	FrustumVolume* frustum = (FrustumVolume*)glMapBufferRange(
		GL_SHADER_STORAGE_BUFFER,0, sizeof(float) * 4 * 36 * 32 * 32,GL_MAP_READ_BIT);
	printf("gpu matrix :\nM4:%f,%f,%f,%f\nM3:%f,%f,%f,%f\nM2:%f,%f,%f,%f\nM1:%f,%f,%f,%f\n",
		frustum[0].CommonPlanes[3].x, frustum[0].CommonPlanes[3].y, frustum[0].CommonPlanes[3].z, frustum[0].CommonPlanes[3].w,
		frustum[0].CommonPlanes[2].x, frustum[0].CommonPlanes[2].y, frustum[0].CommonPlanes[2].z, frustum[0].CommonPlanes[2].w, 
		frustum[0].CommonPlanes[1].x, frustum[0].CommonPlanes[1].y, frustum[0].CommonPlanes[1].z, frustum[0].CommonPlanes[1].w, 
		frustum[0].CommonPlanes[0].x, frustum[0].CommonPlanes[0].y, frustum[0].CommonPlanes[0].z, frustum[0].CommonPlanes[0].w);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	float step = 2.0f / float(sScreenSplit);
	quads = new ScreenQuad*[sScreenSplit * sScreenSplit];
	for (int y = 0; y < sScreenSplit; ++y) {
		for (int x = 0; x < sScreenSplit; ++x) {
			int quad_index = y * sScreenSplit + x;
			FrustumVolume* current_frustum = &frustum[quad_index];
			quads[quad_index] = InitQuad(-1.0f+x*step, -1.0f+y*step,step, 16);
			ScreenQuad* quad = quads[quad_index];
			memcpy(&quad->mCommonPlanes[0], glm::value_ptr(current_frustum->CommonPlanes[0]), sizeof(float) * 4);
			memcpy(&quad->mCommonPlanes[1], glm::value_ptr(current_frustum->CommonPlanes[1]), sizeof(float) * 4);
			memcpy(&quad->mCommonPlanes[2], glm::value_ptr(current_frustum->CommonPlanes[2]), sizeof(float) * 4);
			memcpy(&quad->mCommonPlanes[3], glm::value_ptr(current_frustum->CommonPlanes[3]), sizeof(float) * 4);
			for (int i = 0; i < 16; ++i) {
				NearFarPlanePair* nf = new NearFarPlanePair;
				memcpy(&nf->mNearPlane, glm::value_ptr(current_frustum->NearFarPlanes[i*2]), sizeof(float) * 4);
				memcpy(&nf->mFarPlane, glm::value_ptr(current_frustum->NearFarPlanes[i * 2 + 1]), sizeof(float) * 4);
				quad->mZSplitPlanes.push_back(nf);
			}
			//ExtractPlane(world_to_clip_space, 0, quad->mCommonPlanes[0].mD, &quad->mCommonPlanes[0]);
			//ExtractPlane(world_to_clip_space, 1, quad->mCommonPlanes[1].mD, &quad->mCommonPlanes[1]);
			//ExtractPlane(world_to_clip_space, 2, quad->mCommonPlanes[2].mD, &quad->mCommonPlanes[2]);
			//ExtractPlane(world_to_clip_space, 3, quad->mCommonPlanes[3].mD, &quad->mCommonPlanes[3]);	
			//for (int frustum_index = 0; frustum_index < quad->mZSplitPlanes.size(); ++frustum_index) {
			//	NearFarPlanePair* nf = quad->mZSplitPlanes[frustum_index];
			//	ExtractPlane(world_to_clip_space, 4, nf->mNearPlane.mD, &nf->mNearPlane);
			//	ExtractPlane(world_to_clip_space, 5, nf->mFarPlane.mD, &nf->mFarPlane);
			//}
		}
	}
	for (int y = 0; y < sScreenSplit; ++y) {
		for (int x = 0; x < sScreenSplit; ++x) {
			int quad_index = y * sScreenSplit + x;
			ScreenQuad* quad = quads[quad_index];
			for (auto iter = Light::mLights.begin(); iter != Light::mLights.end(); ++iter) {//iter lights
				Light* current_light = *iter;
				glm::vec3 light_center(current_light->mPosition[0], current_light->mPosition[1],
					current_light->mPosition[2]);
				for (int frustum_index = 0; frustum_index < quad->mZSplitPlanes.size(); ++frustum_index) {
					NearFarPlanePair* nf = quad->mZSplitPlanes[frustum_index];
					Plane frustum[] = {
						quad->mCommonPlanes[0],
						quad->mCommonPlanes[1],
						quad->mCommonPlanes[2],
						quad->mCommonPlanes[3],
						nf->mNearPlane,
						nf->mFarPlane
					};
					if (IntersectSphereFrustumFull(light_center, current_light->mSetting1[0],
						frustum)) {
						//printf("quad(%d:%d) light is in sight!\n", quad_index, frustum_index);
						quad->mLights.insert(current_light);
					}
				}
			}
		}
	}
	for (int y = 0; y < sScreenSplit; ++y) {
		for (int x = 0; x < sScreenSplit; ++x) {
			int quad_index = y * sScreenSplit + x;
			ScreenQuad* quad = quads[quad_index];
			if (quad->mLights.empty()) {
				quad->mLights.insert(black_light);
			}
		}
	}
	fbo = new FrameBufferObject;
	fbo->AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, width, height);
	fbo->AttachDepthBuffer("depth", width, height);
	fbo->Finish();
	positionfbo = new FrameBufferObject;
	positionfbo->AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, width, height,GL_RGBA32F);
	positionfbo->AttachDepthBuffer("depth", width, height);
	positionfbo->Finish();
	normalfbo = new FrameBufferObject;
	normalfbo->AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, width, height, GL_RGBA32F);
	normalfbo->AttachDepthBuffer("depth", width, height);
	normalfbo->Finish();

	colorfbo = new FrameBufferObject;
	colorfbo->AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, width, height);
	colorfbo->AttachDepthBuffer("depth", width, height);
	colorfbo->Finish();
}
void TiledRendering() {
	for (int y = 0; y < sScreenSplit; ++y) {
		for (int x = 0; x < sScreenSplit; ++x) {
			int quad_index = y * sScreenSplit + x;
			ScreenQuad* quad = quads[quad_index];
			quad->mVBO->Bind();
			glUseProgram(fsqshader->mProgram);
			GloblalRenderState::SetDepthMask(true);
			GloblalRenderState::SetBlendState(false);
			fsqshader->mAttributes->Active();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, positionfbo->GetBuffer("color"));
			glUniform1i(glGetUniformLocation(fsqshader->mProgram, "U_PositionTexture"), 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalfbo->GetBuffer("color"));
			glUniform1i(glGetUniformLocation(fsqshader->mProgram, "U_Normalexture"), 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, colorfbo->GetBuffer("color"));
			glUniform1i(glGetUniformLocation(fsqshader->mProgram, "U_ColorTexture"), 2);

			GloblalRenderState::SetDepthMask(false);
			GloblalRenderState::SetBlendState(true);
			GloblalRenderState::SetBlendFunc(GL_ONE, GL_ONE);
			auto iter = quad->mLights.begin();
			auto iter_end = quad->mLights.end();
			while (iter != iter_end) {
				Light* light = *iter;
				glUniform4fv(glGetUniformLocation(fsqshader->mProgram, "U_LightSetting"), 1, light->mSetting0);
				glUniform4fv(glGetUniformLocation(fsqshader->mProgram, "U_LightSetting1"), 1, light->mSetting1);
				glUniform4fv(glGetUniformLocation(fsqshader->mProgram, "U_LightPos"), 1, light->mPosition);
				glUniform4fv(glGetUniformLocation(fsqshader->mProgram, "U_LightColor"), 1, light->mDiffuse);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				++iter;
			}
			quad->mVBO->Unbind();
			glUseProgram(0);
		}
	}
}
void PrepareDefferedGPass(Material*material) {
	DrawCall* current = &dc;
	while (current != nullptr) {
		current->mMaterial = material;
		current = current->Next<DrawCall>();
	}
}
void DefferedShading() {
	PrepareDefferedGPass(deffered_position_material);
	positionfbo->Bind();
	//draw call
	dc.DefferedGPass(&camera);
	positionfbo->Unbind();

	PrepareDefferedGPass(deffered_normal_material);
	normalfbo->Bind();
	//draw call
	dc.DefferedGPass(&camera);
	normalfbo->Unbind();

	PrepareDefferedGPass(deffered_material_color);
	colorfbo->Bind();
	dc.DefferedGPass(&camera);
	colorfbo->Unbind();
	TiledRendering();
}
void DebugScene() {
	//3d
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, sViewportWidth / sViewportHeight, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4ub(255, 255, 255, 255);
	for (auto iter = Light::mLights.begin(); iter != Light::mLights.end(); ++iter) {//iter lights
		Light* light = *iter;
		glPushMatrix();
		glTranslatef(light->mPosition[0], light->mPosition[1], light->mPosition[2]);
		gluSphere(point_light_geometry, light->mSetting1[0], 360, 100);
		glPopMatrix();
	}
	//2d
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-sViewportWidth/2.0f,sViewportWidth/2.0f,-sViewportHeight/2.0f,sViewportHeight/2.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4ub(0,0,255,255);
	glBegin(GL_LINES);
	glVertex2f(-sViewportWidth/2.0f,0.0f);
	glVertex2f(sViewportWidth / 2.0f, 0.0f);
	glVertex2f(0.0f,-sViewportHeight / 2.0f);
	glVertex2f(0.0f,sViewportHeight / 2.0f);
	glEnd();
	//glBegin(GL_POINTS);
	//glVertex2f(
	//	point_light_opengl_screen_space_coordinate.x,
	//	point_light_opengl_screen_space_coordinate.y);
	//glVertex2f(
	//	point_on_sphere_opengl_screen_space_coordinate.x,
	//	point_on_sphere_opengl_screen_space_coordinate.y);
	//glEnd();
}
void Draw() {
	float frameTime = GetFrameTime();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//fbo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//fbo
	//DebugScene();
	DefferedShading();
}