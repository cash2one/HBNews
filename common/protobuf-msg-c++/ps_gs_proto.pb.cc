// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ps_gs_proto.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ps_gs_proto.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* ps_gs_paied_req_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ps_gs_paied_req_reflection_ = NULL;
const ::google::protobuf::Descriptor* ps_gs_paied_req_game_element_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ps_gs_paied_req_game_element_reflection_ = NULL;
const ::google::protobuf::Descriptor* gs_ps_paied_res_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  gs_ps_paied_res_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* e_psgs_proto_version_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* e_msg_ps_gs_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* e_msg_gs_ps_descriptor_ = NULL;
const ::google::protobuf::EnumDescriptor* e_psgs_errorcode_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_ps_5fgs_5fproto_2eproto() {
  protobuf_AddDesc_ps_5fgs_5fproto_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ps_gs_proto.proto");
  GOOGLE_CHECK(file != NULL);
  ps_gs_paied_req_descriptor_ = file->message_type(0);
  static const int ps_gs_paied_req_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req, user_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req, order_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req, eles_),
  };
  ps_gs_paied_req_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ps_gs_paied_req_descriptor_,
      ps_gs_paied_req::default_instance_,
      ps_gs_paied_req_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ps_gs_paied_req));
  ps_gs_paied_req_game_element_descriptor_ = ps_gs_paied_req_descriptor_->nested_type(0);
  static const int ps_gs_paied_req_game_element_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req_game_element, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req_game_element, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req_game_element, num_),
  };
  ps_gs_paied_req_game_element_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ps_gs_paied_req_game_element_descriptor_,
      ps_gs_paied_req_game_element::default_instance_,
      ps_gs_paied_req_game_element_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req_game_element, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ps_gs_paied_req_game_element, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ps_gs_paied_req_game_element));
  gs_ps_paied_res_descriptor_ = file->message_type(1);
  static const int gs_ps_paied_res_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(gs_ps_paied_res, error_code_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(gs_ps_paied_res, order_id_),
  };
  gs_ps_paied_res_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      gs_ps_paied_res_descriptor_,
      gs_ps_paied_res::default_instance_,
      gs_ps_paied_res_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(gs_ps_paied_res, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(gs_ps_paied_res, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(gs_ps_paied_res));
  e_psgs_proto_version_descriptor_ = file->enum_type(0);
  e_msg_ps_gs_descriptor_ = file->enum_type(1);
  e_msg_gs_ps_descriptor_ = file->enum_type(2);
  e_psgs_errorcode_descriptor_ = file->enum_type(3);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ps_5fgs_5fproto_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ps_gs_paied_req_descriptor_, &ps_gs_paied_req::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ps_gs_paied_req_game_element_descriptor_, &ps_gs_paied_req_game_element::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    gs_ps_paied_res_descriptor_, &gs_ps_paied_res::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ps_5fgs_5fproto_2eproto() {
  delete ps_gs_paied_req::default_instance_;
  delete ps_gs_paied_req_reflection_;
  delete ps_gs_paied_req_game_element::default_instance_;
  delete ps_gs_paied_req_game_element_reflection_;
  delete gs_ps_paied_res::default_instance_;
  delete gs_ps_paied_res_reflection_;
}

void protobuf_AddDesc_ps_5fgs_5fproto_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021ps_gs_proto.proto\"\230\001\n\017ps_gs_paied_req\022"
    "\017\n\007user_id\030\001 \002(\r\022\020\n\010order_id\030\002 \002(\t\022+\n\004el"
    "es\030\003 \003(\0132\035.ps_gs_paied_req.game_element\032"
    "5\n\014game_element\022\014\n\004type\030\001 \002(\r\022\n\n\002id\030\002 \002("
    "\r\022\013\n\003num\030\003 \002(\r\"7\n\017gs_ps_paied_res\022\022\n\nerr"
    "or_code\030\001 \002(\005\022\020\n\010order_id\030\002 \001(\t*(\n\024e_psg"
    "s_proto_version\022\020\n\014PSGS_VERSION\020\001*R\n\013e_m"
    "sg_ps_gs\022\023\n\017e_msg_ps_gs_min\020\000\022\031\n\025e_msg_p"
    "s_gs_paied_req\020\001\022\023\n\017e_msg_ps_gs_max\020\005*R\n"
    "\013e_msg_gs_ps\022\023\n\017e_msg_gs_ps_min\020\000\022\031\n\025e_m"
    "sg_gs_ps_paied_res\020\001\022\023\n\017e_msg_gs_ps_max\020"
    "\005*\207\001\n\020e_psgs_errorcode\022\030\n\024e_psgs_errorco"
    "de_min\020\000\022\034\n\030e_psgs_errorcode_success\020\001\022!"
    "\n\035e_psgs_errorcode_unkown_error\020\002\022\030\n\024e_p"
    "sgs_errorcode_max\020\004", 579);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ps_gs_proto.proto", &protobuf_RegisterTypes);
  ps_gs_paied_req::default_instance_ = new ps_gs_paied_req();
  ps_gs_paied_req_game_element::default_instance_ = new ps_gs_paied_req_game_element();
  gs_ps_paied_res::default_instance_ = new gs_ps_paied_res();
  ps_gs_paied_req::default_instance_->InitAsDefaultInstance();
  ps_gs_paied_req_game_element::default_instance_->InitAsDefaultInstance();
  gs_ps_paied_res::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ps_5fgs_5fproto_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ps_5fgs_5fproto_2eproto {
  StaticDescriptorInitializer_ps_5fgs_5fproto_2eproto() {
    protobuf_AddDesc_ps_5fgs_5fproto_2eproto();
  }
} static_descriptor_initializer_ps_5fgs_5fproto_2eproto_;
const ::google::protobuf::EnumDescriptor* e_psgs_proto_version_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return e_psgs_proto_version_descriptor_;
}
bool e_psgs_proto_version_IsValid(int value) {
  switch(value) {
    case 1:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* e_msg_ps_gs_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return e_msg_ps_gs_descriptor_;
}
bool e_msg_ps_gs_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* e_msg_gs_ps_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return e_msg_gs_ps_descriptor_;
}
bool e_msg_gs_ps_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 5:
      return true;
    default:
      return false;
  }
}

const ::google::protobuf::EnumDescriptor* e_psgs_errorcode_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return e_psgs_errorcode_descriptor_;
}
bool e_psgs_errorcode_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 4:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int ps_gs_paied_req_game_element::kTypeFieldNumber;
const int ps_gs_paied_req_game_element::kIdFieldNumber;
const int ps_gs_paied_req_game_element::kNumFieldNumber;
#endif  // !_MSC_VER

ps_gs_paied_req_game_element::ps_gs_paied_req_game_element()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ps_gs_paied_req_game_element::InitAsDefaultInstance() {
}

ps_gs_paied_req_game_element::ps_gs_paied_req_game_element(const ps_gs_paied_req_game_element& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ps_gs_paied_req_game_element::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0u;
  id_ = 0u;
  num_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ps_gs_paied_req_game_element::~ps_gs_paied_req_game_element() {
  SharedDtor();
}

void ps_gs_paied_req_game_element::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ps_gs_paied_req_game_element::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ps_gs_paied_req_game_element::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ps_gs_paied_req_game_element_descriptor_;
}

const ps_gs_paied_req_game_element& ps_gs_paied_req_game_element::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ps_5fgs_5fproto_2eproto();
  return *default_instance_;
}

ps_gs_paied_req_game_element* ps_gs_paied_req_game_element::default_instance_ = NULL;

ps_gs_paied_req_game_element* ps_gs_paied_req_game_element::New() const {
  return new ps_gs_paied_req_game_element;
}

void ps_gs_paied_req_game_element::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0u;
    id_ = 0u;
    num_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ps_gs_paied_req_game_element::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_id;
        break;
      }

      // required uint32 id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &id_)));
          set_has_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_num;
        break;
      }

      // required uint32 num = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_num:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &num_)));
          set_has_num();
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
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ps_gs_paied_req_game_element::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->type(), output);
  }

  // required uint32 id = 2;
  if (has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->id(), output);
  }

  // required uint32 num = 3;
  if (has_num()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->num(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ps_gs_paied_req_game_element::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->type(), target);
  }

  // required uint32 id = 2;
  if (has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->id(), target);
  }

  // required uint32 num = 3;
  if (has_num()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->num(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ps_gs_paied_req_game_element::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->type());
    }

    // required uint32 id = 2;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->id());
    }

    // required uint32 num = 3;
    if (has_num()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->num());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ps_gs_paied_req_game_element::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ps_gs_paied_req_game_element* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ps_gs_paied_req_game_element*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ps_gs_paied_req_game_element::MergeFrom(const ps_gs_paied_req_game_element& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_num()) {
      set_num(from.num());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ps_gs_paied_req_game_element::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ps_gs_paied_req_game_element::CopyFrom(const ps_gs_paied_req_game_element& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ps_gs_paied_req_game_element::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void ps_gs_paied_req_game_element::Swap(ps_gs_paied_req_game_element* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(id_, other->id_);
    std::swap(num_, other->num_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ps_gs_paied_req_game_element::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ps_gs_paied_req_game_element_descriptor_;
  metadata.reflection = ps_gs_paied_req_game_element_reflection_;
  return metadata;
}


// -------------------------------------------------------------------

#ifndef _MSC_VER
const int ps_gs_paied_req::kUserIdFieldNumber;
const int ps_gs_paied_req::kOrderIdFieldNumber;
const int ps_gs_paied_req::kElesFieldNumber;
#endif  // !_MSC_VER

ps_gs_paied_req::ps_gs_paied_req()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ps_gs_paied_req::InitAsDefaultInstance() {
}

ps_gs_paied_req::ps_gs_paied_req(const ps_gs_paied_req& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ps_gs_paied_req::SharedCtor() {
  _cached_size_ = 0;
  user_id_ = 0u;
  order_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ps_gs_paied_req::~ps_gs_paied_req() {
  SharedDtor();
}

void ps_gs_paied_req::SharedDtor() {
  if (order_id_ != &::google::protobuf::internal::kEmptyString) {
    delete order_id_;
  }
  if (this != default_instance_) {
  }
}

void ps_gs_paied_req::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ps_gs_paied_req::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ps_gs_paied_req_descriptor_;
}

const ps_gs_paied_req& ps_gs_paied_req::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ps_5fgs_5fproto_2eproto();
  return *default_instance_;
}

ps_gs_paied_req* ps_gs_paied_req::default_instance_ = NULL;

ps_gs_paied_req* ps_gs_paied_req::New() const {
  return new ps_gs_paied_req;
}

void ps_gs_paied_req::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    user_id_ = 0u;
    if (has_order_id()) {
      if (order_id_ != &::google::protobuf::internal::kEmptyString) {
        order_id_->clear();
      }
    }
  }
  eles_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ps_gs_paied_req::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 user_id = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &user_id_)));
          set_has_user_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_order_id;
        break;
      }

      // required string order_id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_order_id:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_order_id()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->order_id().data(), this->order_id().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_eles;
        break;
      }

      // repeated .ps_gs_paied_req.game_element eles = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_eles:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_eles()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_eles;
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ps_gs_paied_req::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 user_id = 1;
  if (has_user_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->user_id(), output);
  }

  // required string order_id = 2;
  if (has_order_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->order_id().data(), this->order_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->order_id(), output);
  }

  // repeated .ps_gs_paied_req.game_element eles = 3;
  for (int i = 0; i < this->eles_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, this->eles(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ps_gs_paied_req::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 user_id = 1;
  if (has_user_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->user_id(), target);
  }

  // required string order_id = 2;
  if (has_order_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->order_id().data(), this->order_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->order_id(), target);
  }

  // repeated .ps_gs_paied_req.game_element eles = 3;
  for (int i = 0; i < this->eles_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        3, this->eles(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ps_gs_paied_req::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 user_id = 1;
    if (has_user_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->user_id());
    }

    // required string order_id = 2;
    if (has_order_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->order_id());
    }

  }
  // repeated .ps_gs_paied_req.game_element eles = 3;
  total_size += 1 * this->eles_size();
  for (int i = 0; i < this->eles_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->eles(i));
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ps_gs_paied_req::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ps_gs_paied_req* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ps_gs_paied_req*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ps_gs_paied_req::MergeFrom(const ps_gs_paied_req& from) {
  GOOGLE_CHECK_NE(&from, this);
  eles_.MergeFrom(from.eles_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_user_id()) {
      set_user_id(from.user_id());
    }
    if (from.has_order_id()) {
      set_order_id(from.order_id());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ps_gs_paied_req::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ps_gs_paied_req::CopyFrom(const ps_gs_paied_req& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ps_gs_paied_req::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  for (int i = 0; i < eles_size(); i++) {
    if (!this->eles(i).IsInitialized()) return false;
  }
  return true;
}

void ps_gs_paied_req::Swap(ps_gs_paied_req* other) {
  if (other != this) {
    std::swap(user_id_, other->user_id_);
    std::swap(order_id_, other->order_id_);
    eles_.Swap(&other->eles_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ps_gs_paied_req::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ps_gs_paied_req_descriptor_;
  metadata.reflection = ps_gs_paied_req_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int gs_ps_paied_res::kErrorCodeFieldNumber;
const int gs_ps_paied_res::kOrderIdFieldNumber;
#endif  // !_MSC_VER

gs_ps_paied_res::gs_ps_paied_res()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void gs_ps_paied_res::InitAsDefaultInstance() {
}

gs_ps_paied_res::gs_ps_paied_res(const gs_ps_paied_res& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void gs_ps_paied_res::SharedCtor() {
  _cached_size_ = 0;
  error_code_ = 0;
  order_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

gs_ps_paied_res::~gs_ps_paied_res() {
  SharedDtor();
}

void gs_ps_paied_res::SharedDtor() {
  if (order_id_ != &::google::protobuf::internal::kEmptyString) {
    delete order_id_;
  }
  if (this != default_instance_) {
  }
}

void gs_ps_paied_res::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* gs_ps_paied_res::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return gs_ps_paied_res_descriptor_;
}

const gs_ps_paied_res& gs_ps_paied_res::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ps_5fgs_5fproto_2eproto();
  return *default_instance_;
}

gs_ps_paied_res* gs_ps_paied_res::default_instance_ = NULL;

gs_ps_paied_res* gs_ps_paied_res::New() const {
  return new gs_ps_paied_res;
}

void gs_ps_paied_res::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    error_code_ = 0;
    if (has_order_id()) {
      if (order_id_ != &::google::protobuf::internal::kEmptyString) {
        order_id_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool gs_ps_paied_res::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 error_code = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &error_code_)));
          set_has_error_code();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_order_id;
        break;
      }

      // optional string order_id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_order_id:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_order_id()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->order_id().data(), this->order_id().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void gs_ps_paied_res::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 error_code = 1;
  if (has_error_code()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->error_code(), output);
  }

  // optional string order_id = 2;
  if (has_order_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->order_id().data(), this->order_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->order_id(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* gs_ps_paied_res::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 error_code = 1;
  if (has_error_code()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->error_code(), target);
  }

  // optional string order_id = 2;
  if (has_order_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->order_id().data(), this->order_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->order_id(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int gs_ps_paied_res::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 error_code = 1;
    if (has_error_code()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->error_code());
    }

    // optional string order_id = 2;
    if (has_order_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->order_id());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void gs_ps_paied_res::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const gs_ps_paied_res* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const gs_ps_paied_res*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void gs_ps_paied_res::MergeFrom(const gs_ps_paied_res& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_error_code()) {
      set_error_code(from.error_code());
    }
    if (from.has_order_id()) {
      set_order_id(from.order_id());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void gs_ps_paied_res::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void gs_ps_paied_res::CopyFrom(const gs_ps_paied_res& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool gs_ps_paied_res::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void gs_ps_paied_res::Swap(gs_ps_paied_res* other) {
  if (other != this) {
    std::swap(error_code_, other->error_code_);
    std::swap(order_id_, other->order_id_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata gs_ps_paied_res::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = gs_ps_paied_res_descriptor_;
  metadata.reflection = gs_ps_paied_res_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)