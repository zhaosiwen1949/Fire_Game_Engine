#include "RenderOrder.h"
namespace Alice{
	RenderOrder::RenderOrder():mRenderUnit(nullptr){
	}
	RenderOrder::~RenderOrder(){
	}
	void RenderOrder::Update(float deltaTime){
	}
	void RenderOrder::Render(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	){
	}
}
