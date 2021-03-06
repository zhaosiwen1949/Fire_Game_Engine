// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Mesh.serializer.h"

#include <algorithm>

#include "once.h"
#include "coded_stream.h"
#include "wire_format_lite_inl.h"
// @@protoc_insertion_point(includes)

namespace Serializer {

void protobuf_ShutdownFile_Mesh_2eproto() {
  delete Mesh::default_instance_;
}

void protobuf_AddDesc_Mesh_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  Mesh::default_instance_ = new Mesh();
  Mesh::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Mesh_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Mesh_2eproto {
  StaticDescriptorInitializer_Mesh_2eproto() {
    protobuf_AddDesc_Mesh_2eproto();
  }
} static_descriptor_initializer_Mesh_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int Mesh::kVertexCountFieldNumber;
const int Mesh::kVertexBufferFieldNumber;
const int Mesh::kIndexCountFieldNumber;
const int Mesh::kIndexBufferFieldNumber;
const int Mesh::kNameFieldNumber;
#endif  // !_MSC_VER

Mesh::Mesh()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void Mesh::InitAsDefaultInstance() {
}

Mesh::Mesh(const Mesh& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void Mesh::SharedCtor() {
  _cached_size_ = 0;
  vertexcount_ = 0;
  vertexbuffer_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  indexcount_ = 0;
  indexbuffer_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Mesh::~Mesh() {
  SharedDtor();
}

void Mesh::SharedDtor() {
  if (vertexbuffer_ != &::google::protobuf::internal::kEmptyString) {
    delete vertexbuffer_;
  }
  if (indexbuffer_ != &::google::protobuf::internal::kEmptyString) {
    delete indexbuffer_;
  }
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (this != default_instance_) {
  }
}

void Mesh::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const Mesh& Mesh::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Mesh_2eproto();  return *default_instance_;
}

Mesh* Mesh::default_instance_ = NULL;

Mesh* Mesh::New() const {
  return new Mesh;
}

void Mesh::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    vertexcount_ = 0;
    if (has_vertexbuffer()) {
      if (vertexbuffer_ != &::google::protobuf::internal::kEmptyString) {
        vertexbuffer_->clear();
      }
    }
    indexcount_ = 0;
    if (has_indexbuffer()) {
      if (indexbuffer_ != &::google::protobuf::internal::kEmptyString) {
        indexbuffer_->clear();
      }
    }
    if (has_name()) {
      if (name_ != &::google::protobuf::internal::kEmptyString) {
        name_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool Mesh::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 vertexCount = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &vertexcount_)));
          set_has_vertexcount();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_vertexBuffer;
        break;
      }
      
      // optional bytes vertexBuffer = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_vertexBuffer:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_vertexbuffer()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_indexCount;
        break;
      }
      
      // optional int32 indexCount = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_indexCount:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &indexcount_)));
          set_has_indexcount();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_indexBuffer;
        break;
      }
      
      // optional bytes indexBuffer = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_indexBuffer:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_indexbuffer()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_name;
        break;
      }
      
      // optional string name = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_name:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_name()));
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

void Mesh::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 vertexCount = 1;
  if (has_vertexcount()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->vertexcount(), output);
  }
  
  // optional bytes vertexBuffer = 2;
  if (has_vertexbuffer()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->vertexbuffer(), output);
  }
  
  // optional int32 indexCount = 3;
  if (has_indexcount()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->indexcount(), output);
  }
  
  // optional bytes indexBuffer = 4;
  if (has_indexbuffer()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      4, this->indexbuffer(), output);
  }
  
  // optional string name = 5;
  if (has_name()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->name(), output);
  }
  
}

int Mesh::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 vertexCount = 1;
    if (has_vertexcount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->vertexcount());
    }
    
    // optional bytes vertexBuffer = 2;
    if (has_vertexbuffer()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->vertexbuffer());
    }
    
    // optional int32 indexCount = 3;
    if (has_indexcount()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->indexcount());
    }
    
    // optional bytes indexBuffer = 4;
    if (has_indexbuffer()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->indexbuffer());
    }
    
    // optional string name = 5;
    if (has_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->name());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Mesh::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const Mesh*>(&from));
}

void Mesh::MergeFrom(const Mesh& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_vertexcount()) {
      set_vertexcount(from.vertexcount());
    }
    if (from.has_vertexbuffer()) {
      set_vertexbuffer(from.vertexbuffer());
    }
    if (from.has_indexcount()) {
      set_indexcount(from.indexcount());
    }
    if (from.has_indexbuffer()) {
      set_indexbuffer(from.indexbuffer());
    }
    if (from.has_name()) {
      set_name(from.name());
    }
  }
}

void Mesh::CopyFrom(const Mesh& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Mesh::IsInitialized() const {
  
  return true;
}

void Mesh::Swap(Mesh* other) {
  if (other != this) {
    std::swap(vertexcount_, other->vertexcount_);
    std::swap(vertexbuffer_, other->vertexbuffer_);
    std::swap(indexcount_, other->indexcount_);
    std::swap(indexbuffer_, other->indexbuffer_);
    std::swap(name_, other->name_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string Mesh::GetTypeName() const {
  return "Serializer.Mesh";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Serializer

// @@protoc_insertion_point(global_scope)
