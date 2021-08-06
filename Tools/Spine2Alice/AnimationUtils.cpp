#include "utils.h"
#include "SpineUtils.h"
Serializer::TwoColorAnimationClip*ConvertTwoColorTimelineToAlice(spTimeline*tl) {
	Serializer::TwoColorAnimationClip*ac = new Serializer::TwoColorAnimationClip;
	spTwoColorTimeline*tctl = SUB_CAST(spTwoColorTimeline, tl);
	int key_frame_count = tctl->framesCount / 8;
	for (int i = 0; i < key_frame_count; i++) {
		int offset = i * 8;
		printf("slot(%d) time(%f) \n",tctl->slotIndex,tctl->frames[offset]);
		Serializer::TwoColorAnimationKeyFrame*key_frame = ac->add_key_frames();
		key_frame->set_time(tctl->frames[offset]);
		key_frame->add_light(tctl->frames[offset + 1]);
		key_frame->add_light(tctl->frames[offset + 2]);
		key_frame->add_light(tctl->frames[offset + 3]);
		key_frame->add_light(tctl->frames[offset + 4]);
		key_frame->add_dark(tctl->frames[offset + 5]);
		key_frame->add_dark(tctl->frames[offset + 6]);
		key_frame->add_dark(tctl->frames[offset + 7]);
		if (i < key_frame_count - 1) {//last frame do not have a curve
			if (tctl->super.curves[19 * i] == 0.0f) {//linear
				key_frame->set_interpolation_type(0);
			}
			else if (tctl->super.curves[19 * i] == 1.0f) {//step
				key_frame->set_interpolation_type(1);
			}
			else if (tctl->super.curves[19 * i] == 2.0f) {//bezier
				key_frame->set_interpolation_type(2);
				key_frame->add_external_args(tctl->super.curves[19 * i + 1]);
				key_frame->add_external_args(tctl->super.curves[19 * i + 2]);
				key_frame->add_external_args(tctl->super.curves[19 * i + 3]);
				key_frame->add_external_args(tctl->super.curves[19 * i + 4]);
			}
		}
		else {//last key frame
			key_frame->set_interpolation_type(1);
		}
	}
	return ac;
}
Serializer::SpriteSequenceAnimationClip*ConvertAttachmentTimelineToAlice(spTimeline*tl) {
	Serializer::SpriteSequenceAnimationClip*ac = new Serializer::SpriteSequenceAnimationClip;
	spAttachmentTimeline * atl = SUB_CAST(spAttachmentTimeline, tl);
	for (int i=0;i<atl->framesCount;++i){
		Serializer::SpriteSequenceAnimationKeyFrame * ssackf = ac->add_key_frames();
		ssackf->set_time(atl->frames[i]);
		if (atl->attachmentNames[i]!=nullptr){
			ssackf->set_image_name(atl->attachmentNames[i]);
		}
		else {
			ssackf->set_image_name("alice_none");
		}
		
	}
	return ac;
}
Serializer::ColorAnimationClip*ConvertColorTimelineToAlice(spTimeline*tl) {
	Serializer::ColorAnimationClip*ac = new Serializer::ColorAnimationClip;
	spColorTimeline * ctl = SUB_CAST(spColorTimeline, tl);
	int key_frame_count = ctl->framesCount / COLOR_ENTRIES;
	for (int i = 0; i < key_frame_count; ++i) {
		Serializer::ColorAnimationKeyFrame * cackf = ac->add_key_frames();
		cackf->set_time(ctl->frames[i*COLOR_ENTRIES]);
		cackf->add_color(ctl->frames[i*COLOR_ENTRIES + 1]);
		cackf->add_color(ctl->frames[i*COLOR_ENTRIES + 2]);
		cackf->add_color(ctl->frames[i*COLOR_ENTRIES + 3]);
		cackf->add_color(ctl->frames[i*COLOR_ENTRIES + 4]);
		if (i < key_frame_count - 1) {//last frame do not have a curve
			if (ctl->super.curves[19 * i] == 0.0f) {//linear
				cackf->set_interpolation_type(0);
			}
			else if (ctl->super.curves[19 * i] == 1.0f) {//step
				cackf->set_interpolation_type(1);
			}
			else if (ctl->super.curves[19 * i] == 2.0f) {//bezier
				cackf->set_interpolation_type(2);
				cackf->add_external_args(ctl->super.curves[19 * i + 1]);
				cackf->add_external_args(ctl->super.curves[19 * i + 2]);
				cackf->add_external_args(ctl->super.curves[19 * i + 3]);
				cackf->add_external_args(ctl->super.curves[19 * i + 4]);
			}
		}
		else {//last key frame
			cackf->set_interpolation_type(1);
		}
	}
	return ac;
}
Serializer::TranslateAnimationClip*ConvertTranslateTimelineToAlice(spTimeline*tl) {
	Serializer::TranslateAnimationClip*ac = new Serializer::TranslateAnimationClip;
	spTranslateTimeline * ttl = SUB_CAST(spTranslateTimeline, tl);
	int key_frame_count = ttl->framesCount / TRANSLATE_ENTRIES;
	for (int i = 0; i < key_frame_count; ++i) {
		Serializer::TranslateAnimationKeyFrame * kf = ac->add_key_frames();
		kf->set_time(ttl->frames[i*TRANSLATE_ENTRIES]);
		kf->add_translation(ttl->frames[i*TRANSLATE_ENTRIES + 1]);
		kf->add_translation(ttl->frames[i*TRANSLATE_ENTRIES + 2]);
		kf->add_translation(0.0f);
		kf->add_translation(1.0f);
		if (i < key_frame_count - 1) {//last frame do not have a curve
			if (ttl->super.curves[19 * i] == 0.0f) {//linear
				kf->set_interpolation_type(0);
			}
			else if (ttl->super.curves[19 * i] == 1.0f) {//step
				kf->set_interpolation_type(1);
			}
			else if (ttl->super.curves[19 * i] == 2.0f) {//bezier
				kf->set_interpolation_type(2);
				kf->add_external_args(ttl->super.curves[19 * i + 1]);
				kf->add_external_args(ttl->super.curves[19 * i + 2]);
				kf->add_external_args(ttl->super.curves[19 * i + 3]);
				kf->add_external_args(ttl->super.curves[19 * i + 4]);
			}
		}
		else {//last key frame
			kf->set_interpolation_type(1);
		}
	}
	return ac;
}
Serializer::RotateAnimationClip*ConvertRotationTimelineToAlice(spTimeline*tl) {
	Serializer::RotateAnimationClip*ac = new Serializer::RotateAnimationClip;
	spRotateTimeline * ttl = SUB_CAST(spRotateTimeline, tl);
	int key_frame_count = ttl->framesCount / ROTATE_ENTRIES;
	for (int i = 0; i < key_frame_count; ++i) {
		Serializer::RotateAnimationKeyFrame * kf = ac->add_key_frames();
		kf->set_time(ttl->frames[i*ROTATE_ENTRIES]);
		kf->add_rotation(0.0f);
		kf->add_rotation(0.0f);
		float angle = ttl->frames[i*ROTATE_ENTRIES + 1];
		if (angle>0.0f){
			if (angle>180.0f){
				angle -= 360.0f;
			}
		}
		else {
			if (angle<-180.0f){
				angle += 360.0f;
			}
		}
		kf->add_rotation(angle);
		if (i < key_frame_count - 1) {//last frame do not have a curve
			if (ttl->super.curves[19 * i] == 0.0f) {//linear
				kf->set_interpolation_type(0);
			}
			else if (ttl->super.curves[19 * i] == 1.0f) {//step
				kf->set_interpolation_type(1);
			}
			else if (ttl->super.curves[19 * i] == 2.0f) {//bezier
				kf->set_interpolation_type(2);
				kf->add_external_args(ttl->super.curves[19 * i + 1]);
				kf->add_external_args(ttl->super.curves[19 * i + 2]);
				kf->add_external_args(ttl->super.curves[19 * i + 3]);
				kf->add_external_args(ttl->super.curves[19 * i + 4]);
			}
		}
		else {//last key frame
			kf->set_interpolation_type(1);
		}
	}
	return ac;
}
Serializer::ScaleAnimationClip*ConvertScaleTimelineToAlice(spTimeline*tl) {\
	Serializer::ScaleAnimationClip*ac = new Serializer::ScaleAnimationClip;
	spScaleTimeline * ttl = SUB_CAST(spScaleTimeline, tl);
	int key_frame_count = ttl->framesCount / TRANSLATE_ENTRIES;
	for (int i = 0; i < key_frame_count; ++i) {
		Serializer::ScaleAnimationKeyFrame * kf = ac->add_key_frames();
		kf->set_time(ttl->frames[i*TRANSLATE_ENTRIES]);
		kf->add_scale(ttl->frames[i*TRANSLATE_ENTRIES + 1]);
		kf->add_scale(ttl->frames[i*TRANSLATE_ENTRIES + 2]);
		kf->add_scale(1.0f);
		if (i < key_frame_count - 1) {//last frame do not have a curve
			if (ttl->super.curves[19 * i] == 0.0f) {//linear
				kf->set_interpolation_type(0);
			}
			else if (ttl->super.curves[19 * i] == 1.0f) {//step
				kf->set_interpolation_type(1);
			}
			else if (ttl->super.curves[19 * i] == 2.0f) {//bezier
				kf->set_interpolation_type(2);
				kf->add_external_args(ttl->super.curves[19 * i + 1]);
				kf->add_external_args(ttl->super.curves[19 * i + 2]);
				kf->add_external_args(ttl->super.curves[19 * i + 3]);
				kf->add_external_args(ttl->super.curves[19 * i + 4]);
			}
		}
		else {//last key frame
			kf->set_interpolation_type(1);
		}
	}
	return ac;
}
Serializer::ShearAnimationClip*ConvertShearingTimelineToAlice(spTimeline*tl) {
	Serializer::ShearAnimationClip*ac = new Serializer::ShearAnimationClip;
	spShearTimeline * ttl = SUB_CAST(spShearTimeline, tl);
	int key_frame_count = ttl->framesCount / TRANSLATE_ENTRIES;
	for (int i = 0; i < key_frame_count; ++i) {
		Serializer::ShearAnimationKeyFrame * kf = ac->add_key_frames();
		kf->set_time(ttl->frames[i*TRANSLATE_ENTRIES]);
		kf->add_shear(ttl->frames[i*TRANSLATE_ENTRIES + 1]);
		kf->add_shear(ttl->frames[i*TRANSLATE_ENTRIES + 2]);
		kf->add_shear(0.0f);
		if (i < key_frame_count - 1) {//last frame do not have a curve
			if (ttl->super.curves[19 * i] == 0.0f) {//linear
				kf->set_interpolation_type(0);
			}
			else if (ttl->super.curves[19 * i] == 1.0f) {//step
				kf->set_interpolation_type(1);
			}
			else if (ttl->super.curves[19 * i] == 2.0f) {//bezier
				kf->set_interpolation_type(2);
				kf->add_external_args(ttl->super.curves[19 * i + 1]);
				kf->add_external_args(ttl->super.curves[19 * i + 2]);
				kf->add_external_args(ttl->super.curves[19 * i + 3]);
				kf->add_external_args(ttl->super.curves[19 * i + 4]);
			}
		}
		else {//last key frame
			kf->set_interpolation_type(1);
		}
	}
	return ac;
}
Serializer::DrawOrderAnimationClip*ConvertDrawOrderTimelineToAlice(spSkeletonData*skeleton, spTimeline*tl) {
	Serializer::DrawOrderAnimationClip*ac = new Serializer::DrawOrderAnimationClip;
	spDrawOrderTimeline * ttl = SUB_CAST(spDrawOrderTimeline, tl);
	for (int i = 0; i < ttl->framesCount; ++i) {
		Serializer::DrawOrderAnimationKeyFrame * kf = ac->add_key_frames();
		kf->set_time(ttl->frames[i]);
		if (ttl->drawOrders[i] != nullptr) {
			printf("draw order time : %f ", ttl->frames[i]);
			for (int k = 0; k < ttl->slotsCount; ++k) {
				printf(" %d", ttl->drawOrders[i][k]);
				spSlotData*slot = GetSlot(skeleton, ttl->drawOrders[i][k]);
				if (slot != nullptr) {
					char slot_name[256] = { 0 };
					sprintf(slot_name, "s_%s", slot->name);
					kf->add_draw_order(slot_name);
				}
				else {
					printf("cannot fint draw order target %d", ttl->drawOrders[i][k]);
				}
			}
			printf("\n");
		}
		else {
			printf("draw order time : %f original draw order\n", ttl->frames[i]);
		}
	}
	return ac;
}