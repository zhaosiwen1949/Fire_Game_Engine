#include "RenderOrderAnimation.h"
#include "Runtime/Scene/GameObject.h"
#include "Runtime/Render/RenderOrder.h"
namespace Alice {
	void RenderOrderAnimationKeyFrame::Init(AliceAny root, const Serializer::DrawOrderAnimationKeyFrame&key_frame_data) {
		mTime = key_frame_data.time();
		if (key_frame_data.draw_order_size() > 0) {
			mRenderOrder = new RenderOrder;
			for (int i = 0; i < key_frame_data.draw_order_size(); ++i) {
				GameObject*go = ((GameObject*)root)->FindChild(key_frame_data.draw_order(i).c_str());
				if (go == nullptr) {
					Error("cannot find render order target %s", key_frame_data.draw_order(i).c_str());
				}
				else {
					RenderUnit*ru = new RenderUnit;
					ru->mIndex = i;
					ru->mOffset = 0;
					ru->mGameObject = go;
					if (mRenderOrder->mRenderUnit == nullptr) {
						mRenderOrder->mRenderUnit = ru;
					}
					else {
						mRenderOrder->mRenderUnit->Append(ru);
					}
				}
			}
		}
		else {
			mRenderOrder = ((GameObject*)root)->mRenderOrder;
		}
		mInterpolateType = kAnimationInterpolateTypeStep;
	}
	void RenderOrderAnimation::OnUpdateStepAnimation(AnimationKeyFrame*keyframe) {
		GameObject*target = (GameObject*)mAnimateTarget;
		target->mRenderOrder = ((RenderOrderAnimationKeyFrame*)keyframe)->mRenderOrder;
	}
	void RenderOrderAnimation::OnUpdateLinearAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
	}
	void RenderOrderAnimation::OnUpdateBezierAnimation(float animation_time, AnimationKeyFrame*start_frame, AnimationKeyFrame*end_frame) {
	}
}