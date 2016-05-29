// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: tile_flags.proto

#ifndef PROTOBUF_tile_5fflags_2eproto__INCLUDED
#define PROTOBUF_tile_5fflags_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_tile_5fflags_2eproto();
void protobuf_AssignDesc_tile_5fflags_2eproto();
void protobuf_ShutdownFile_tile_5fflags_2eproto();

class tile_flags;

// ===================================================================

class tile_flags : public ::google::protobuf::Message {
 public:
  tile_flags();
  virtual ~tile_flags();

  tile_flags(const tile_flags& from);

  inline tile_flags& operator=(const tile_flags& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const tile_flags& default_instance();

  void Swap(tile_flags* other);

  // implements Message ----------------------------------------------

  inline tile_flags* New() const { return New(NULL); }

  tile_flags* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const tile_flags& from);
  void MergeFrom(const tile_flags& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(tile_flags* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 num_tiles = 1;
  void clear_num_tiles();
  static const int kNumTilesFieldNumber = 1;
  ::google::protobuf::int32 num_tiles() const;
  void set_num_tiles(::google::protobuf::int32 value);

  // repeated bool tile_frame_important = 2 [packed = true];
  int tile_frame_important_size() const;
  void clear_tile_frame_important();
  static const int kTileFrameImportantFieldNumber = 2;
  bool tile_frame_important(int index) const;
  void set_tile_frame_important(int index, bool value);
  void add_tile_frame_important(bool value);
  const ::google::protobuf::RepeatedField< bool >&
      tile_frame_important() const;
  ::google::protobuf::RepeatedField< bool >*
      mutable_tile_frame_important();

  // repeated bool tile_solid = 3 [packed = true];
  int tile_solid_size() const;
  void clear_tile_solid();
  static const int kTileSolidFieldNumber = 3;
  bool tile_solid(int index) const;
  void set_tile_solid(int index, bool value);
  void add_tile_solid(bool value);
  const ::google::protobuf::RepeatedField< bool >&
      tile_solid() const;
  ::google::protobuf::RepeatedField< bool >*
      mutable_tile_solid();

  // @@protoc_insertion_point(class_scope:tile_flags)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedField< bool > tile_frame_important_;
  mutable int _tile_frame_important_cached_byte_size_;
  ::google::protobuf::RepeatedField< bool > tile_solid_;
  mutable int _tile_solid_cached_byte_size_;
  ::google::protobuf::int32 num_tiles_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_tile_5fflags_2eproto();
  friend void protobuf_AssignDesc_tile_5fflags_2eproto();
  friend void protobuf_ShutdownFile_tile_5fflags_2eproto();

  void InitAsDefaultInstance();
  static tile_flags* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// tile_flags

// optional int32 num_tiles = 1;
inline void tile_flags::clear_num_tiles() {
  num_tiles_ = 0;
}
inline ::google::protobuf::int32 tile_flags::num_tiles() const {
  // @@protoc_insertion_point(field_get:tile_flags.num_tiles)
  return num_tiles_;
}
inline void tile_flags::set_num_tiles(::google::protobuf::int32 value) {
  
  num_tiles_ = value;
  // @@protoc_insertion_point(field_set:tile_flags.num_tiles)
}

// repeated bool tile_frame_important = 2 [packed = true];
inline int tile_flags::tile_frame_important_size() const {
  return tile_frame_important_.size();
}
inline void tile_flags::clear_tile_frame_important() {
  tile_frame_important_.Clear();
}
inline bool tile_flags::tile_frame_important(int index) const {
  // @@protoc_insertion_point(field_get:tile_flags.tile_frame_important)
  return tile_frame_important_.Get(index);
}
inline void tile_flags::set_tile_frame_important(int index, bool value) {
  tile_frame_important_.Set(index, value);
  // @@protoc_insertion_point(field_set:tile_flags.tile_frame_important)
}
inline void tile_flags::add_tile_frame_important(bool value) {
  tile_frame_important_.Add(value);
  // @@protoc_insertion_point(field_add:tile_flags.tile_frame_important)
}
inline const ::google::protobuf::RepeatedField< bool >&
tile_flags::tile_frame_important() const {
  // @@protoc_insertion_point(field_list:tile_flags.tile_frame_important)
  return tile_frame_important_;
}
inline ::google::protobuf::RepeatedField< bool >*
tile_flags::mutable_tile_frame_important() {
  // @@protoc_insertion_point(field_mutable_list:tile_flags.tile_frame_important)
  return &tile_frame_important_;
}

// repeated bool tile_solid = 3 [packed = true];
inline int tile_flags::tile_solid_size() const {
  return tile_solid_.size();
}
inline void tile_flags::clear_tile_solid() {
  tile_solid_.Clear();
}
inline bool tile_flags::tile_solid(int index) const {
  // @@protoc_insertion_point(field_get:tile_flags.tile_solid)
  return tile_solid_.Get(index);
}
inline void tile_flags::set_tile_solid(int index, bool value) {
  tile_solid_.Set(index, value);
  // @@protoc_insertion_point(field_set:tile_flags.tile_solid)
}
inline void tile_flags::add_tile_solid(bool value) {
  tile_solid_.Add(value);
  // @@protoc_insertion_point(field_add:tile_flags.tile_solid)
}
inline const ::google::protobuf::RepeatedField< bool >&
tile_flags::tile_solid() const {
  // @@protoc_insertion_point(field_list:tile_flags.tile_solid)
  return tile_solid_;
}
inline ::google::protobuf::RepeatedField< bool >*
tile_flags::mutable_tile_solid() {
  // @@protoc_insertion_point(field_mutable_list:tile_flags.tile_solid)
  return &tile_solid_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_tile_5fflags_2eproto__INCLUDED