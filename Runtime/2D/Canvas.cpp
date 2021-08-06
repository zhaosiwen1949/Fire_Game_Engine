#include "Canvas.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Scene/SceneManager.h"
namespace Alice {
	std::vector<Canvas*> Canvas::mCanvases;
	Canvas::Canvas() {
		mCanvases.push_back(this);
		int width = 0, height = 0;
		GetSceneManager()->GetCanvasSize(width, height);
		Set(width, height, 100.0f);
		mOrder = 0;
	}
	Canvas::~Canvas() {
		auto iter = mCanvases.begin();
		auto iterEnd = mCanvases.end();
		for (;iterEnd!=iter;++iter){
			if (*iter==this){
				mCanvases.erase(iter);
				break;
			}
		}
	}
	void Canvas::Set(float width, float height, float depth) {
		float half_width = width / 2.0f;
		float half_height = height / 2.0f;
		mCanvasInfo.Set(half_width, half_height, depth);
		mRect.Set(-half_width, -half_height, width, height);
	}
	void Canvas::SetDepthRange(float depth) {
		mCanvasInfo.z = depth;
	}
	void Canvas::SetOrder(int order) {
		mOrder = order;
		std::sort(mCanvases.begin(), mCanvases.end(), Canvas::SortCanvas);
	}
	void Canvas::OnViewportChanged(float width, float height) {
		auto iter = mCanvases.begin();
		auto iterEnd = mCanvases.end();
		for (; iterEnd != iter; ++iter) {
			Canvas*current_canvas = *iter;
			current_canvas->Set(width / 2.0f, height / 2.0f, current_canvas->mCanvasInfo.z);
			if (current_canvas->mOwner!=nullptr){
				current_canvas->mOwner->OnViewportChanged(width,height);
			}
		}
	}
}