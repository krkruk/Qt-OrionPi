// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: testMsg.proto

#ifndef PROTOBUF_testMsg_2eproto__INCLUDED
#define PROTOBUF_testMsg_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace NetTest {
class TestMsg;
class TestMsgDefaultTypeInternal;
extern TestMsgDefaultTypeInternal _TestMsg_default_instance_;
}  // namespace NetTest

namespace NetTest {

namespace protobuf_testMsg_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_testMsg_2eproto

enum TestMsg_Sample {
  TestMsg_Sample_Unknown = 0,
  TestMsg_Sample_First = 1,
  TestMsg_Sample_Second = 2,
  TestMsg_Sample_TestMsg_Sample_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  TestMsg_Sample_TestMsg_Sample_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool TestMsg_Sample_IsValid(int value);
const TestMsg_Sample TestMsg_Sample_Sample_MIN = TestMsg_Sample_Unknown;
const TestMsg_Sample TestMsg_Sample_Sample_MAX = TestMsg_Sample_Second;
const int TestMsg_Sample_Sample_ARRAYSIZE = TestMsg_Sample_Sample_MAX + 1;

const ::google::protobuf::EnumDescriptor* TestMsg_Sample_descriptor();
inline const ::std::string& TestMsg_Sample_Name(TestMsg_Sample value) {
  return ::google::protobuf::internal::NameOfEnum(
    TestMsg_Sample_descriptor(), value);
}
inline bool TestMsg_Sample_Parse(
    const ::std::string& name, TestMsg_Sample* value) {
  return ::google::protobuf::internal::ParseNamedEnum<TestMsg_Sample>(
    TestMsg_Sample_descriptor(), name, value);
}
// ===================================================================

class TestMsg : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:NetTest.TestMsg) */ {
 public:
  TestMsg();
  virtual ~TestMsg();

  TestMsg(const TestMsg& from);

  inline TestMsg& operator=(const TestMsg& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TestMsg& default_instance();

  static inline const TestMsg* internal_default_instance() {
    return reinterpret_cast<const TestMsg*>(
               &_TestMsg_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(TestMsg* other);

  // implements Message ----------------------------------------------

  inline TestMsg* New() const PROTOBUF_FINAL { return New(NULL); }

  TestMsg* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const TestMsg& from);
  void MergeFrom(const TestMsg& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(TestMsg* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  typedef TestMsg_Sample Sample;
  static const Sample Unknown =
    TestMsg_Sample_Unknown;
  static const Sample First =
    TestMsg_Sample_First;
  static const Sample Second =
    TestMsg_Sample_Second;
  static inline bool Sample_IsValid(int value) {
    return TestMsg_Sample_IsValid(value);
  }
  static const Sample Sample_MIN =
    TestMsg_Sample_Sample_MIN;
  static const Sample Sample_MAX =
    TestMsg_Sample_Sample_MAX;
  static const int Sample_ARRAYSIZE =
    TestMsg_Sample_Sample_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Sample_descriptor() {
    return TestMsg_Sample_descriptor();
  }
  static inline const ::std::string& Sample_Name(Sample value) {
    return TestMsg_Sample_Name(value);
  }
  static inline bool Sample_Parse(const ::std::string& name,
      Sample* value) {
    return TestMsg_Sample_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // string text = 2;
  void clear_text();
  static const int kTextFieldNumber = 2;
  const ::std::string& text() const;
  void set_text(const ::std::string& value);
  #if LANG_CXX11
  void set_text(::std::string&& value);
  #endif
  void set_text(const char* value);
  void set_text(const char* value, size_t size);
  ::std::string* mutable_text();
  ::std::string* release_text();
  void set_allocated_text(::std::string* text);

  // int64 integerValue = 3;
  void clear_integervalue();
  static const int kIntegerValueFieldNumber = 3;
  ::google::protobuf::int64 integervalue() const;
  void set_integervalue(::google::protobuf::int64 value);

  // .NetTest.TestMsg.Sample sample = 1;
  void clear_sample();
  static const int kSampleFieldNumber = 1;
  ::NetTest::TestMsg_Sample sample() const;
  void set_sample(::NetTest::TestMsg_Sample value);

  // @@protoc_insertion_point(class_scope:NetTest.TestMsg)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr text_;
  ::google::protobuf::int64 integervalue_;
  int sample_;
  mutable int _cached_size_;
  friend struct protobuf_testMsg_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// TestMsg

// .NetTest.TestMsg.Sample sample = 1;
inline void TestMsg::clear_sample() {
  sample_ = 0;
}
inline ::NetTest::TestMsg_Sample TestMsg::sample() const {
  // @@protoc_insertion_point(field_get:NetTest.TestMsg.sample)
  return static_cast< ::NetTest::TestMsg_Sample >(sample_);
}
inline void TestMsg::set_sample(::NetTest::TestMsg_Sample value) {
  
  sample_ = value;
  // @@protoc_insertion_point(field_set:NetTest.TestMsg.sample)
}

// string text = 2;
inline void TestMsg::clear_text() {
  text_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TestMsg::text() const {
  // @@protoc_insertion_point(field_get:NetTest.TestMsg.text)
  return text_.GetNoArena();
}
inline void TestMsg::set_text(const ::std::string& value) {
  
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:NetTest.TestMsg.text)
}
#if LANG_CXX11
inline void TestMsg::set_text(::std::string&& value) {
  
  text_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:NetTest.TestMsg.text)
}
#endif
inline void TestMsg::set_text(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:NetTest.TestMsg.text)
}
inline void TestMsg::set_text(const char* value, size_t size) {
  
  text_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:NetTest.TestMsg.text)
}
inline ::std::string* TestMsg::mutable_text() {
  
  // @@protoc_insertion_point(field_mutable:NetTest.TestMsg.text)
  return text_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TestMsg::release_text() {
  // @@protoc_insertion_point(field_release:NetTest.TestMsg.text)
  
  return text_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TestMsg::set_allocated_text(::std::string* text) {
  if (text != NULL) {
    
  } else {
    
  }
  text_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), text);
  // @@protoc_insertion_point(field_set_allocated:NetTest.TestMsg.text)
}

// int64 integerValue = 3;
inline void TestMsg::clear_integervalue() {
  integervalue_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 TestMsg::integervalue() const {
  // @@protoc_insertion_point(field_get:NetTest.TestMsg.integerValue)
  return integervalue_;
}
inline void TestMsg::set_integervalue(::google::protobuf::int64 value) {
  
  integervalue_ = value;
  // @@protoc_insertion_point(field_set:NetTest.TestMsg.integerValue)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace NetTest

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::NetTest::TestMsg_Sample> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NetTest::TestMsg_Sample>() {
  return ::NetTest::TestMsg_Sample_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_testMsg_2eproto__INCLUDED
