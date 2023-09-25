#pragma once

#include <cstddef>
#include <cstdint>

class ITerminalReceiveCharacterCallback
{
public:
  virtual void OnReceiveCharacter(char c) = 0;
  virtual ~ITerminalReceiveCharacterCallback() = default;
};
