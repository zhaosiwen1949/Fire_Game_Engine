#pragma once
extern "C" {
#include "spine/spine.h"
#include "spine/extension.h"
}
#include "TextureInfo.serializer.h"
#include "ImageSprite.serializer.h"
#include "Animation.serializer.h"
#include "Avatar.serializer.h"
Serializer::TwoColorAnimationClip * ConvertTwoColorTimelineToAlice(spTimeline*tl);
Serializer::SpriteSequenceAnimationClip * ConvertAttachmentTimelineToAlice(spTimeline*tl);
Serializer::ColorAnimationClip * ConvertColorTimelineToAlice(spTimeline*tl);
Serializer::TranslateAnimationClip * ConvertTranslateTimelineToAlice(spTimeline*tl);
Serializer::RotateAnimationClip * ConvertRotationTimelineToAlice(spTimeline*tl);
Serializer::ScaleAnimationClip * ConvertScaleTimelineToAlice(spTimeline*tl);
Serializer::ShearAnimationClip * ConvertShearingTimelineToAlice(spTimeline*tl);
Serializer::DrawOrderAnimationClip * ConvertDrawOrderTimelineToAlice(spSkeletonData*skeleton, spTimeline*tl);
