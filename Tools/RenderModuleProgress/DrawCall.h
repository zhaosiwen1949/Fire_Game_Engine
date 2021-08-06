#pragma once
#include "AliceGL.h"
#include "LinkedList.h"
#include "Material.h"
#include "GL20/VBO.h"
#include "GL20/IBO.h"
enum RenderCatalog {
	kRenderCatalogForward,
	kRenderCatalogDefferedGPass,
	kRenderCatalogDefferedLightPass,
	kRenderCatalogCount
};
class DrawCall :public LinkedList {
public:
	Material*mMaterial;
	VBO *mVBO;
	IBO *mIBO;
	RenderCatalog mCatalog;
	DrawCall();
	void Draw(Camera*camera);
	void ForwardRendering(Camera* camera);
	void DefferedGPass(Camera* camera);
	void DefferedLightPass(Camera* camera);
};