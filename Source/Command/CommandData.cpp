#include "CommandData.h"

#include <cassert>
#include <cctype>
#include <cstring>
#include <ios>
#include <string>

using namespace ::command;

CommandData::RawInput& CommandData::GetRawInput()
{
  return mRawInput;
}

ComponentName CommandData::GetComponentName() const
{
  return mComponentName;
}

void CommandData::SetComponentName(ComponentName name)
{
  assert(name < ComponentName::count);
  mComponentName = name;
}

const char* CommandData::GetCommand() const
{
  return &mRawInput[mCommandIndex];
}

bool CommandData::CommandIs(const char* command) const
{
  return strncmp(command, GetCommand(), MaxCommandLength) == 0;
}

size_t CommandData::GetParameterCount() const
{
  if (mPairCount <= MandatoryPairCount)
    return 0;

  return mPairCount - MandatoryPairCount;
}

bool CommandData::ValidateCommand()
{
  size_t commandIndex = 0;
  if (!GetValueIndex(CommandKey, commandIndex))
    return false;

  mCommandIndex = commandIndex;
  return true;
}

bool CommandData::AddKeyValuePair(const KeyValuePair& pair)
{
  if (mPairCount >= MaxKeyValuePairs)
    return false;

  if (!IsValidChar(mRawInput[pair.KeyIndex])
    || !IsValidChar(mRawInput[pair.ValueIndex]))
    return false;

  if (mPairCount >= MandatoryPairCount && DuplicateKeyExists(pair))
    return false;

  mPairs[mPairCount++] = pair;
  return true;
}

size_t CommandData::GetKeyValuePairCount() const
{
  return mPairCount;
}

bool CommandData::GetString(const char* key, const char*& outString) const
{
  size_t valueIndex = 0;
  if (!GetValueIndex(key, valueIndex))
    return false;

  outString = &mRawInput[valueIndex];
  return true;
}

bool CommandData::GetUint(const char* key, uint32_t& outInt) const
{
  size_t valueIndex = 0;
  if (!GetValueIndex(key, valueIndex))
    return false;

  const char* valueString = &mRawInput[valueIndex];

  if (!std::isdigit(valueString[0]))
    return false;

  const uint32_t base = 10;
  char* nextChar = nullptr;
  outInt = strtoul(valueString, &nextChar, base);

  if (*nextChar != '\0')
  { // Ensure int is not part of a word
    return false;
  }

  return true;
}

bool CommandData::GetInt(const char* key, int32_t& outInt) const
{
  size_t valueIndex = 0;
  if (!GetValueIndex(key, valueIndex))
    return false;

  const char* valueString = &mRawInput[valueIndex];
  const auto firstChar = valueString[0];

  if (!std::isdigit(firstChar) && firstChar != '-')
    return false;

  const uint32_t base = 10;
  char* nextChar = nullptr;
  outInt = strtol(valueString, &nextChar, base);

  if (*nextChar != '\0')
  { // Ensure int is not part of a word
    return false;
  }

  return true;
}

bool CommandData::GetValueIndex(const char* key, size_t& outIndex) const
{
  for (size_t i = 0; i < mPairCount; ++i)
  {
    if (strncmp(key, &mRawInput[mPairs[i].KeyIndex], MaxKeyLength) == 0)
    {
      outIndex = mPairs[i].ValueIndex;
      return true;
    }
  }
  return false;
}

bool CommandData::DuplicateKeyExists(const KeyValuePair& pair) const
{
  const auto currentParameterName = &mRawInput[pair.KeyIndex];
  for (size_t i = 0; i < mPairCount; i++)
  {
    const auto previousParameterName = &mRawInput[mPairs[i].KeyIndex];
    if (strncmp(currentParameterName, previousParameterName, MaxKeyLength) == 0)
      return true;
  }
  return false;
}

bool CommandData::IsValidChar(const char& c) const
{
  return std::isalnum(c) || std::ispunct(c);
}