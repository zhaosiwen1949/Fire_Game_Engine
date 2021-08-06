#pragma once
#include "Runtime/Base/Component.h"
namespace Alice{
	class RenderUnit :public DoubleLinkedList {
	public:
		GameObject*mGameObject;
		int mIndex;
		int mOffset;
	};
	class RenderOrder :public Component {
	public:
		RenderOrder();
		virtual ~RenderOrder();
		virtual void Update(float deltaTime);
		virtual void Render(RenderQueue*rq
#if ALICE_EDITOR
			, DrawCallInfo &rs
#endif
		);
	public:
		RenderUnit*mRenderUnit;
	public:
		DECLEAR_ALICE_CLASS(RenderOrder)
	};
}