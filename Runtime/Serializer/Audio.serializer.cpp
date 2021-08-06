// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Audio.serializer.h"

#include <algorithm>

#include "once.h"
#include "coded_stream.h"
#include "wire_format_lite_inl.h"
// @@protoc_insertion_point(includes)

namespace Serializer {

void protobuf_ShutdownFile_Audio_2eproto() {
  delete RuntimeAudioData::default_instance_;
}

void protobuf_AddDesc_Audio_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  RuntimeAudioData::default_instance_ = new RuntimeAudioData();
  RuntimeAudioData::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Audio_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Audio_2eproto {
  StaticDescriptorInitializer_Audio_2eproto() {
    protobuf_AddDesc_Audio_2eproto();
  }
} static_descriptor_initializer_Audio_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int RuntimeAudioData::kPathFieldNumber;
const int RuntimeAudioData::kFormatFieldNumber;
const int RuntimeAudioData::kFrequenceFieldNumber;
const int RuntimeAudioData::kBytePerSampleFieldNumber;
const int RuntimeAudioData::kChannelCountFieldNumber;
const int RuntimeAudioData::kOpenalFormatFieldNumber;
const int RuntimeAudioData::kDataFieldNumber;
#endif  // !_MSC_VER

RuntimeAudioData::RuntimeAudioData()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void RuntimeAudioData::InitAsDefaultInstance() {
}

RuntimeAudioData::RuntimeAudioData(const RuntimeAudioData& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void RuntimeAudioData::SharedCtor() {
  _cached_size_ = 0;
  path_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  format_ = 0;
  frequence_ = 0;
  byte_per_sample_ = 0;
  channel_count_ = 0;
  openal_format_ = 0;
  data_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RuntimeAudioData::~RuntimeAudioData() {
  SharedDtor();
}

void RuntimeAudioData::SharedDtor() {
  if (path_ != &::google::protobuf::internal::kEmptyString) {
    delete path_;
  }
  if (data_ != &::google::protobuf::internal::kEmptyString) {
    delete data_;
  }
  if (this != default_instance_) {
  }
}

void RuntimeAudioData::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const RuntimeAudioData& RuntimeAudioData::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Audio_2eproto();  return *default_instance_;
}

RuntimeAudioData* RuntimeAudioData::default_instance_ = NULL;

RuntimeAudioData* RuntimeAudioData::New() const {
  return new RuntimeAudioData;
}

void RuntimeAudioData::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_path()) {
      if (path_ != &::google::protobuf::internal::kEmptyString) {
        path_->clear();
      }
    }
    format_ = 0;
    frequence_ = 0;
    byte_per_sample_ = 0;
    channel_count_ = 0;
    openal_format_ = 0;
    if (has_data()) {
      if (data_ != &::google::protobuf::internal::kEmptyString) {
        data_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool RuntimeAudioData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string path = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_path()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_format;
        break;
      }
      
      // optional int32 format = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_format:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &format_)));
          set_has_format();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_frequence;
        break;
      }
      
      // optional int32 frequence = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_frequence:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &frequence_)));
          set_has_frequence();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_byte_per_sample;
        break;
      }
      
      // optional int32 byte_per_sample = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_byte_per_sample:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &byte_per_sample_)));
          set_has_byte_per_sample();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_channel_count;
        break;
      }
      
      // optional int32 channel_count = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_channel_count:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &channel_count_)));
          set_has_channel_count();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(48)) goto parse_openal_format;
        break;
      }
      
      // optional int32 openal_format = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_openal_format:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &openal_format_)));
          set_has_openal_format();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_data;
        break;
      }
      
      // optional bytes data = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_data:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_data()));
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

void RuntimeAudioData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string path = 1;
  if (has_path()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->path(), output);
  }
  
  // optional int32 format = 2;
  if (has_format()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->format(), output);
  }
  
  // optional int32 frequence = 3;
  if (has_frequence()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->frequence(), output);
  }
  
  // optional int32 byte_per_sample = 4;
  if (has_byte_per_sample()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->byte_per_sample(), output);
  }
  
  // optional int32 channel_count = 5;
  if (has_channel_count()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->channel_count(), output);
  }
  
  // optional int32 openal_format = 6;
  if (has_openal_format()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(6, this->openal_format(), output);
  }
  
  // optional bytes data = 7;
  if (has_data()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      7, this->data(), output);
  }
  
}

int RuntimeAudioData::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string path = 1;
    if (has_path()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->path());
    }
    
    // optional int32 format = 2;
    if (has_format()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->format());
    }
    
    // optional int32 frequence = 3;
    if (has_frequence()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->frequence());
    }
    
    // optional int32 byte_per_sample = 4;
    if (has_byte_per_sample()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->byte_per_sample());
    }
    
    // optional int32 channel_count = 5;
    if (has_channel_count()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->channel_count());
    }
    
    // optional int32 openal_format = 6;
    if (has_openal_format()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->openal_format());
    }
    
    // optional bytes data = 7;
    if (has_data()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->data());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RuntimeAudioData::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const RuntimeAudioData*>(&from));
}

void RuntimeAudioData::MergeFrom(const RuntimeAudioData& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_path()) {
      set_path(from.path());
    }
    if (from.has_format()) {
      set_format(from.format());
    }
    if (from.has_frequence()) {
      set_frequence(from.frequence());
    }
    if (from.has_byte_per_sample()) {
      set_byte_per_sample(from.byte_per_sample());
    }
    if (from.has_channel_count()) {
      set_channel_count(from.channel_count());
    }
    if (from.has_openal_format()) {
      set_openal_format(from.openal_format());
    }
    if (from.has_data()) {
      set_data(from.data());
    }
  }
}

void RuntimeAudioData::CopyFrom(const RuntimeAudioData& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RuntimeAudioData::IsInitialized() const {
  
  return true;
}

void RuntimeAudioData::Swap(RuntimeAudioData* other) {
  if (other != this) {
    std::swap(path_, other->path_);
    std::swap(format_, other->format_);
    std::swap(frequence_, other->frequence_);
    std::swap(byte_per_sample_, other->byte_per_sample_);
    std::swap(channel_count_, other->channel_count_);
    std::swap(openal_format_, other->openal_format_);
    std::swap(data_, other->data_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string RuntimeAudioData::GetTypeName() const {
  return "Serializer.RuntimeAudioData";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Serializer

// @@protoc_insertion_point(global_scope)