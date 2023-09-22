#pragma once

#include "ComponentName.h"
#include "ICommandData.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace command
{

struct KeyValuePair
{
  size_t KeyIndex;
  size_t ValueIndex;
};

class CommandData final : public ICommandData
{
public:
  static constexpr size_t MaxRawInputLength = 128;
  typedef std::array<char, MaxRawInputLength> RawInput;
  static constexpr size_t MaxKeyValuePairs = 10;
  static constexpr size_t MandatoryPairCount = 2;
  static constexpr size_t MaxParameterCount =
    MaxKeyValuePairs - MandatoryPairCount;
  static constexpr size_t MaxKeyLength = 32;

  // ICommandData
  bool GetString(const char* key, const char*& outString) const override;
  bool GetUint(const char* key, uint32_t& outInt) const override;
  ComponentName GetComponentName() const override;
  const char* GetCommand() const override;
  bool CommandIs(const char* command) const override;
  size_t GetParameterCount() const override;

  bool AddKeyValuePair(const KeyValuePair& pair);
  bool ValidateCommand();
  RawInput& GetRawInput();
  size_t GetKeyValuePairCount() const;
  void SetComponentName(ComponentName name);

private:
  static constexpr char const* CommandKey = "command";
  static constexpr size_t MaxCommandLength = 16;

  RawInput mRawInput {};
  ComponentName mComponentName = ComponentName::none;
  size_t mCommandIndex = 0;
  size_t mPairCount = 0;
  std::array<KeyValuePair, MaxKeyValuePairs> mPairs {};

  bool GetValueIndex(const char* key, size_t& outIndex) const;
  bool DuplicateKeyExists(const KeyValuePair& pair) const;
};

}
