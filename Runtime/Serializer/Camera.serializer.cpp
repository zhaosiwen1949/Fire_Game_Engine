// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Camera.serializer.h"

#include <algorithm>

#include "once.h"
#include "coded_stream.h"
#include "wire_format_lite_inl.h"
// @@protoc_insertion_point(includes)

namespace Serializer {

void protobuf_ShutdownFile_Camera_2eproto() {
  delete Camera::default_instance_;
}

void protobuf_AddDesc_Camera_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::Serializer::protobuf_AddDesc_Color_2eproto();
  Camera::default_instance_ = new Camera();
  Camera::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Camera_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Camera_2eproto {
  StaticDescriptorInitializer_Camera_2eproto() {
    protobuf_AddDesc_Camera_2eproto();
  }
} static_descriptor_initializer_Camera_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int Camera::kProjectionModeFieldNumber;
const int Camera::kClearModeFieldNumber;
const int Camera::kClipNearFieldNumber;
const int Camera::kClipFarFieldNumber;
const int Camera::kDepthFieldNumber;
const int Camera::kCullingMaskFieldNumber;
const int Camera::kOrthModeSizeFieldNumber;
const int Camera::kFovFieldNumber;
const int Camera::kRenderToDepthFieldNumber;
const int Camera::kRenderToOffscreenFieldNumber;
const int Camera::kClearColorFieldNumber;
const int Camera::kSkyboxFieldNumber;
#endif  // !_MSC_VER

Camera::Camera()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void Camera::InitAsDefaultInstance() {
  clear_color_ = const_cast< ::Serializer::Color4B*>(&::Serializer::Color4B::default_instance());
}

Camera::Camera(const Camera& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void Camera::SharedCtor() {
  _cached_size_ = 0;
  projection_mode_ = 0;
  clear_mode_ = 0;
  clip_near_ = 0;
  clip_far_ = 0;
  depth_ = 0;
  culling_mask_ = 0u;
  orth_mode_size_ = 0;
  fov_ = 0;
  render_to_depth_ = 0;
  render_to_offscreen_ = 0;
  clear_color_ = NULL;
  skybox_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Camera::~Camera() {
  SharedDtor();
}

void Camera::SharedDtor() {
  if (skybox_ != &::google::protobuf::internal::kEmptyString) {
    delete skybox_;
  }
  if (this != default_instance_) {
    delete clear_color_;
  }
}

void Camera::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const Camera& Camera::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Camera_2eproto();  return *default_instance_;
}

Camera* Camera::default_instance_ = NULL;

Camera* Camera::New() const {
  return new Camera;
}

void Camera::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    projection_mode_ = 0;
    clear_mode_ = 0;
    clip_near_ = 0;
    clip_far_ = 0;
    depth_ = 0;
    culling_mask_ = 0u;
    orth_mode_size_ = 0;
    fov_ = 0;
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    render_to_depth_ = 0;
    render_to_offscreen_ = 0;
    if (has_clear_color()) {
      if (clear_color_ != NULL) clear_color_->::Serializer::Color4B::Clear();
    }
    if (has_skybox()) {
      if (skybox_ != &::google::protobuf::internal::kEmptyString) {
        skybox_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool Camera::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 projection_mode = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &projection_mode_)));
          set_has_projection_mode();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_clear_mode;
        break;
      }
      
      // optional int32 clear_mode = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_clear_mode:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &clear_mode_)));
          set_has_clear_mode();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(29)) goto parse_clip_near;
        break;
      }
      
      // optional float clip_near = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_clip_near:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &clip_near_)));
          set_has_clip_near();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(37)) goto parse_clip_far;
        break;
      }
      
      // optional float clip_far = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_clip_far:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &clip_far_)));
          set_has_clip_far();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(45)) goto parse_depth;
        break;
      }
      
      // optional float depth = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_depth:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &depth_)));
          set_has_depth();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_culling_mask;
        break;
      }
      
      // optional uint32 culling_mask = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_culling_mask:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &culling_mask_)));
          set_has_culling_mask();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(61)) goto parse_orth_mode_size;
        break;
      }
      
      // optional float orth_mode_size = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_orth_mode_size:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &orth_mode_size_)));
          set_has_orth_mode_size();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(69)) goto parse_fov;
        break;
      }
      
      // optional float fov = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_fov:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &fov_)));
          set_has_fov();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(72)) goto parse_render_to_depth;
        break;
      }
      
      // optional int32 render_to_depth = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_render_to_depth:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &render_to_depth_)));
          set_has_render_to_depth();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(80)) goto parse_render_to_offscreen;
        break;
      }
      
      // optional int32 render_to_offscreen = 10;
      case 10: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_render_to_offscreen:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &render_to_offscreen_)));
          set_has_render_to_offscreen();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(90)) goto parse_clear_color;
        break;
      }
      
      // optional .Serializer.Color4B clear_color = 11;
      case 11: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_clear_color:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_clear_color()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(98)) goto parse_skybox;
        break;
      }
      
      // optional string skybox = 12;
      case 12: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_skybox:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_skybox()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Camera::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 projection_mode = 1;
  if (has_projection_mode()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->projection_mode(), output);
  }
  
  // optional int32 clear_mode = 2;
  if (has_clear_mode()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->clear_mode(), output);
  }
  
  // optional float clip_near = 3;
  if (has_clip_near()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(3, this->clip_near(), output);
  }
  
  // optional float clip_far = 4;
  if (has_clip_far()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(4, this->clip_far(), output);
  }
  
  // optional float depth = 5;
  if (has_depth()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(5, this->depth(), output);
  }
  
  // optional uint32 culling_mask = 6;
  if (has_culling_mask()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(6, this->culling_mask(), output);
  }
  
  // optional float orth_mode_size = 7;
  if (has_orth_mode_size()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(7, this->orth_mode_size(), output);
  }
  
  // optional float fov = 8;
  if (has_fov()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(8, this->fov(), output);
  }
  
  // optional int32 render_to_depth = 9;
  if (has_render_to_depth()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(9, this->render_to_depth(), output);
  }
  
  // optional int32 render_to_offscreen = 10;
  if (has_render_to_offscreen()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(10, this->render_to_offscreen(), output);
  }
  
  // optional .Serializer.Color4B clear_color = 11;
  if (has_clear_color()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      11, this->clear_color(), output);
  }
  
  // optional string skybox = 12;
  if (has_skybox()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      12, this->skybox(), output);
  }
  
}

int Camera::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 projection_mode = 1;
    if (has_projection_mode()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->projection_mode());
    }
    
    // optional int32 clear_mode = 2;
    if (has_clear_mode()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->clear_mode());
    }
    
    // optional float clip_near = 3;
    if (has_clip_near()) {
      total_size += 1 + 4;
    }
    
    // optional float clip_far = 4;
    if (has_clip_far()) {
      total_size += 1 + 4;
    }
    
    // optional float depth = 5;
    if (has_depth()) {
      total_size += 1 + 4;
    }
    
    // optional uint32 culling_mask = 6;
    if (has_culling_mask()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->culling_mask());
    }
    
    // optional float orth_mode_size = 7;
    if (has_orth_mode_size()) {
      total_size += 1 + 4;
    }
    
    // optional float fov = 8;
    if (has_fov()) {
      total_size += 1 + 4;
    }
    
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // optional int32 render_to_depth = 9;
    if (has_render_to_depth()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->render_to_depth());
    }
    
    // optional int32 render_to_offscreen = 10;
    if (has_render_to_offscreen()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->render_to_offscreen());
    }
    
    // optional .Serializer.Color4B clear_color = 11;
    if (has_clear_color()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->clear_color());
    }
    
    // optional string skybox = 12;
    if (has_skybox()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->skybox());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Camera::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const Camera*>(&from));
}

void Camera::MergeFrom(const Camera& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_projection_mode()) {
      set_projection_mode(from.projection_mode());
    }
    if (from.has_clear_mode()) {
      set_clear_mode(from.clear_mode());
    }
    if (from.has_clip_near()) {
      set_clip_near(from.clip_near());
    }
    if (from.has_clip_far()) {
      set_clip_far(from.clip_far());
    }
    if (from.has_depth()) {
      set_depth(from.depth());
    }
    if (from.has_culling_mask()) {
      set_culling_mask(from.culling_mask());
    }
    if (from.has_orth_mode_size()) {
      set_orth_mode_size(from.orth_mode_size());
    }
    if (from.has_fov()) {
      set_fov(from.fov());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from.has_render_to_depth()) {
      set_render_to_depth(from.render_to_depth());
    }
    if (from.has_render_to_offscreen()) {
      set_render_to_offscreen(from.render_to_offscreen());
    }
    if (from.has_clear_color()) {
      mutable_clear_color()->::Serializer::Color4B::MergeFrom(from.clear_color());
    }
    if (from.has_skybox()) {
      set_skybox(from.skybox());
    }
  }
}

void Camera::CopyFrom(const Camera& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Camera::IsInitialized() const {
  
  if (has_clear_color()) {
    if (!this->clear_color().IsInitialized()) return false;
  }
  return true;
}

void Camera::Swap(Camera* other) {
  if (other != this) {
    std::swap(projection_mode_, other->projection_mode_);
    std::swap(clear_mode_, other->clear_mode_);
    std::swap(clip_near_, other->clip_near_);
    std::swap(clip_far_, other->clip_far_);
    std::swap(depth_, other->depth_);
    std::swap(culling_mask_, other->culling_mask_);
    std::swap(orth_mode_size_, other->orth_mode_size_);
    std::swap(fov_, other->fov_);
    std::swap(render_to_depth_, other->render_to_depth_);
    std::swap(render_to_offscreen_, other->render_to_offscreen_);
    std::swap(clear_color_, other->clear_color_);
    std::swap(skybox_, other->skybox_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string Camera::GetTypeName() const {
  return "Serializer.Camera";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Serializer

// @@protoc_insertion_point(global_scope)
