// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RELOAD_EFFEKSEER_DATA_H_
#define FLATBUFFERS_GENERATED_RELOAD_EFFEKSEER_DATA_H_

#include "flatbuffers/flatbuffers.h"

namespace Effekseer {
namespace Data {

struct NetworkReload;
struct NetworkReloadBuilder;

struct NetworkReload FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef NetworkReloadBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_KEY = 4,
    VT_DATA = 6
  };
  const flatbuffers::Vector<uint16_t> *key() const {
    return GetPointer<const flatbuffers::Vector<uint16_t> *>(VT_KEY);
  }
  const flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_KEY) &&
           verifier.VerifyVector(key()) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct NetworkReloadBuilder {
  typedef NetworkReload Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_key(flatbuffers::Offset<flatbuffers::Vector<uint16_t>> key) {
    fbb_.AddOffset(NetworkReload::VT_KEY, key);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(NetworkReload::VT_DATA, data);
  }
  explicit NetworkReloadBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<NetworkReload> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<NetworkReload>(end);
    return o;
  }
};

inline flatbuffers::Offset<NetworkReload> CreateNetworkReload(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint16_t>> key = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0) {
  NetworkReloadBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_key(key);
  return builder_.Finish();
}

inline flatbuffers::Offset<NetworkReload> CreateNetworkReloadDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint16_t> *key = nullptr,
    const std::vector<uint8_t> *data = nullptr) {
  auto key__ = key ? _fbb.CreateVector<uint16_t>(*key) : 0;
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return Effekseer::Data::CreateNetworkReload(
      _fbb,
      key__,
      data__);
}

inline const Effekseer::Data::NetworkReload *GetNetworkReload(const void *buf) {
  return flatbuffers::GetRoot<Effekseer::Data::NetworkReload>(buf);
}

inline const Effekseer::Data::NetworkReload *GetSizePrefixedNetworkReload(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<Effekseer::Data::NetworkReload>(buf);
}

inline bool VerifyNetworkReloadBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Effekseer::Data::NetworkReload>(nullptr);
}

inline bool VerifySizePrefixedNetworkReloadBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Effekseer::Data::NetworkReload>(nullptr);
}

inline void FinishNetworkReloadBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Effekseer::Data::NetworkReload> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedNetworkReloadBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Effekseer::Data::NetworkReload> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Data
}  // namespace Effekseer

#endif  // FLATBUFFERS_GENERATED_RELOAD_EFFEKSEER_DATA_H_
