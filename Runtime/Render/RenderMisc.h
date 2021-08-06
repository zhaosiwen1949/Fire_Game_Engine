#pragma once
#include "AliceGL.h"
namespace Alice{
	enum PremitiveType{
		kPremitiveTypeTriangle,
		kPremitiveTypeTriangleStrip,
		kPremitiveTypeTriangleFan,
		kPremitiveTypePoints,
		kPremitiveTypeLines,
		kPremitiveTypeLineStrip,
		kPremitiveTypeLineLoop,
		kPremitiveTypeCount
	};
	enum RenderingQueue {
		kRenderingQueueSkyBox = 1,
		kRenderingQueueOpaque = 500,
		kRenderingQueueTransparent = 1000,
		kRenderingQueueRenderQueueCount
	};
	enum LightingPass {
		kLightingPassNone = 1,
		kLightingPassOnePass = 2,
		kLightingPassForwardBase = 3,
		kLightingPassForwardAdditive = 4,
		kLightingPassDeffered = 5
	};
	enum RenderArchitecture{
		kRenderArchitectureForward = 1,
		kRenderArchitectureDefered,
		kRenderArchitectureCount
	};
	enum RenderPassCatagory {
		kRenderPassCatagoryNoLit = 1,
		kRenderPassCatagoryOnePass,
		kRenderPassCatagoryForwardBase,
		kRenderPassCatagoryForwardAdd,
		kRenderPassCatagoryShadowCaster,
		kRenderPassCatagoryProjector,
		kRenderPassCatagoryDepthMap,
		kRenderPassCatagoryUI,
		kRenderPassCatagoryCount
	};
#define ALICE_ONE GL_ONE
#define ALICE_ZERO GL_ZERO
#define ALICE_SRC_ALPHA GL_SRC_ALPHA
#define ALICE_SRC_COLOR GL_SRC_COLOR
#define ALICE_DST_ALPHA GL_DST_ALPHA
#define ALICE_DST_COLOR GL_DST_COLOR
#define ALICE_ONE_MINUS_SRC_COLOR GL_ONE_MINUS_SRC_COLOR
#define ALICE_ONE_MINUS_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
#define ALICE_ONE_MINUS_DST_COLOR GL_ONE_MINUS_DST_COLOR
#define ALICE_ONE_MINUS_DST_ALPHA GL_ONE_MINUS_DST_ALPHA
}