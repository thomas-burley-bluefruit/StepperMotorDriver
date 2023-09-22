#include "DataQueue.h"
#include <cstring>

using namespace ::utility;

DataQueue::DataQueue(uint8_t* buffer, size_t size) :
  Size(size),
  mHead(0),
  mTail(0),
  mBuffer(buffer)
{
}

size_t DataQueue::Count() const
{
  const auto head = mHead;
  const auto tail = mTail;
  return (Size + head - tail) % Size;
}

size_t DataQueue::FreeCount() const
{
  const auto head = mHead;
  const auto tail = mTail;
  return (head == tail ? Size : (Size + tail - head) % Size) - 1;
}

void DataQueue::Clear()
{
  mHead = 0;
  mTail = 0;
}

bool DataQueue::Read(uint8_t& data)
{
  const size_t head = mHead;
  size_t tail = mTail;
  if (tail == head)
    return false;

  data = mBuffer[tail];
  tail = (tail + 1) % Size;
  mTail = tail;
  return true;
}

size_t DataQueue::ReadBlock(uint8_t* data, size_t size)
{
  if (size == 0)
    return 0;

  // Take snapshot to ensure consistency
  const size_t head = mHead;
  const size_t tail = mTail;
  const size_t count = (Size + head - tail) % Size;
  if (count == 0)
    return 0;

  size_t readSize = size <= count ? size : count;
  size_t newTail = tail + readSize;
  if ((head > tail) || (newTail < Size))
  {
    // Single linear read
    memcpy(data, mBuffer + tail, readSize);
    mTail = newTail;
    return readSize;
  }

  // Split read: to buffer end, then from the start
  const size_t block1Size = Size - tail;
  const size_t block2Size = readSize - block1Size;
  memcpy(data, mBuffer + tail, block1Size);
  memcpy(data + block1Size, mBuffer, block2Size);
  mTail = block2Size;
  return readSize;
}

bool DataQueue::Write(uint8_t data)
{
  const size_t nextHead = (mHead + 1) % Size;
  const size_t tail = mTail;
  if (nextHead == tail)
    return false;

  mBuffer[mHead] = data;
  mHead = nextHead;
  return true;
}

size_t DataQueue::WriteBlock(const uint8_t* data, size_t size)
{
  if (size == 0)
    return 0;

  // Take snapshot to ensure consistency
  const size_t head = mHead;
  const size_t tail = mTail;
  const size_t free = (tail == head ? Size : (Size + tail - head) % Size) - 1;
  if (free == 0)
    return 0;

  size_t writeSize = size <= free ? size : free;
  size_t newHead = head + writeSize;
  if ((head < tail) || (newHead < Size))
  {
    // Single linear write
    memcpy(mBuffer + head, data, writeSize);
    mHead = newHead;
    return writeSize;
  }

  // Split write: to buffer end, then from the start
  const size_t block1Size = Size - head;
  const size_t block2Size = writeSize - block1Size;
  memcpy(mBuffer + head, data, block1Size);
  memcpy(mBuffer, data + block1Size, block2Size);
  mHead = block2Size;
  return writeSize;
}

bool DataQueue::Peek(uint8_t& data, size_t offset) const
{
  if (offset >= Count())
    return false;

  const size_t offsetIndex = (mTail + offset) % Size;
  data = mBuffer[offsetIndex];
  return true;
}

size_t DataQueue::Find(uint8_t data, size_t offset) const
{
  size_t index = offset;
  const size_t count = Count();
  while (index < count)
  {
    const size_t bufferIndex = (mTail + index) % Size;
    if (mBuffer[bufferIndex] == data)
      return index;

    index++;
  }

  return NotFound;
}

size_t DataQueue::ReverseFind(uint8_t data, size_t endOffset) const
{
  const auto count = Count();
  if (count <= endOffset)
    return NotFound;

  size_t lastIndex = count - 1 - endOffset;
  while (true)
  {
    const size_t bufferIndex = (mTail + lastIndex) % Size;
    if (mBuffer[bufferIndex] == data)
      return lastIndex;

    if (lastIndex == 0)
      break;

    lastIndex--;
  }

  return NotFound;
}

size_t DataQueue::DiscardTail(size_t size)
{
  const size_t count = Count();
  const size_t head = mHead;
  size_t tail = mTail;

  if ((tail == head) || (size == 0))
    return 0;

  const size_t bytesToRemove = size <= count ? size : count;
  tail = (tail + bytesToRemove) % Size;

  mTail = tail;
  return bytesToRemove;
}

size_t DataQueue::DiscardHead(size_t size)
{
  const size_t count = Count();
  size_t head = mHead;

  if ((count == 0) || (size == 0))
    return 0;

  const size_t bytesToRemove = size <= count ? size : count;
  head = (head + Size - bytesToRemove) % Size;

  mHead = head;
  return bytesToRemove;
}

const uint8_t* DataQueue::Data() const
{
  return (const uint8_t*)&mBuffer[mTail];
}