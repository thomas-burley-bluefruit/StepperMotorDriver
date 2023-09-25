#pragma once

#include <cstddef>
#include <cstdint>

namespace utility
{

// To ensure consistency:
//   Read and Discard functions must be called from a single thread/point
//   Write functions must be called from a single thread/point (can be same as Read)
class DataQueue
{
public:
  static constexpr size_t NotFound = SIZE_MAX;
  const size_t Size;

  DataQueue(uint8_t* buffer, size_t size);
  void Clear();
  size_t Count() const;
  size_t FreeCount() const;
  bool Read(uint8_t& data);
  size_t ReadBlock(uint8_t* data, size_t size);
  bool Write(uint8_t data);
  size_t WriteBlock(const uint8_t* data, size_t size);
  bool Peek(uint8_t& data, size_t offset) const;
  size_t Find(uint8_t data, size_t offset) const;
  size_t ReverseFind(uint8_t data, size_t endOffset) const;
  size_t DiscardTail(size_t size);
  size_t DiscardHead(size_t size);
  const uint8_t* Data() const;

private:
  volatile size_t mHead;
  volatile size_t mTail;
  uint8_t* mBuffer;
};

}
