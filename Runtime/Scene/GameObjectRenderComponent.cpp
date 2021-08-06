#include "Runtime/Scene/GameObject.h"
#include "Runtime/Render/Camera.h"
#include "Runtime/Render/RenderOrder.h"
#include "Runtime/Render/DrawCall.h"
namespace Alice{
	void GameObject::RenderComponentRecursively(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
		, bool render_sibling) {
		if (mbEnable && (rq->mCamera->mCullingFlags&mLayer) != 0) {
			Component*current = mComponents;
			while (current != nullptr) {
				current->Render(rq
#if ALICE_EDITOR
					, rs
#endif
				);
				current = current->Next<Component>();
			}
		}
		//if parent is disabled,then child is automatically disabled
		if (mbEnable) {
			if (mChild != nullptr) {
				if (mRenderOrder == nullptr){
					Child<GameObject>()->RenderComponentRecursively(rq
#if ALICE_EDITOR
						, rs
#endif
					);
				}
				else {
					RenderUnit*ru = mRenderOrder->mRenderUnit;
					while (ru != nullptr) {
						ru->mGameObject->RenderComponentRecursively( rq
#if ALICE_EDITOR
							, rs
#endif
							, false);
						ru = ru->Next<RenderUnit>();
					}
				}
			}
		}
		if (render_sibling&&mRightSibling != nullptr){
			RightSibling<GameObject>()->RenderComponentRecursively( rq
#if ALICE_EDITOR
				, rs
#endif
			);
		}
	}
	void GameObject::RenderUIComponentRecursively(RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
		, bool render_sibling) {
		if (mbEnable) {
			Component*current = mComponents;
			while (current != nullptr) {
				current->Render(rq
#if ALICE_EDITOR
					, rs
#endif
				);
				current = current->Next<Component>();
			}
		}
		//if parent is disabled,then child is automatically disabled
		if (mbEnable) {
			if (mChild != nullptr) {
				if (mRenderOrder == nullptr) {
					Child<GameObject>()->RenderUIComponentRecursively(rq
#if ALICE_EDITOR
						, rs
#endif
					);
				}
				else {
					RenderUnit*ru = mRenderOrder->mRenderUnit;
					while (ru != nullptr) {
						ru->mGameObject->RenderUIComponentRecursively(rq
#if ALICE_EDITOR
							, rs
#endif
							, false);
						ru = ru->Next<RenderUnit>();
					}
				}
			}
		}
		if (render_sibling&&mRightSibling != nullptr) {
			RightSibling<GameObject>()->RenderUIComponentRecursively(rq
#if ALICE_EDITOR
				, rs
#endif
			);
		}
	}
	void GameObject::RenderShadowMapRecursively(Camera*camera, RenderQueue*rq
#if ALICE_EDITOR
		, DrawCallInfo &rs
#endif
	) {
		if (mbEnable && (camera->mCullingFlags&mLayer) != 0) {
			Component*current = mComponents;
			while (current != nullptr) {
				current->RenderShadowMap(rq
#if ALICE_EDITOR
					, rs
#endif
				);
				current = current->Next<Component>();
			}
		}
		//if parent is disabled,then child is automatically disabled
		if (mbEnable) {
			if (mChild != nullptr) {
				Child<GameObject>()->RenderShadowMapRecursively(camera, rq
#if ALICE_EDITOR
						, rs
#endif
				);
			}
		}
		if (mRightSibling != nullptr) {
			RightSibling<GameObject>()->RenderShadowMapRecursively(camera, rq
#if ALICE_EDITOR
				, rs
#endif
			);
		}
	}
}