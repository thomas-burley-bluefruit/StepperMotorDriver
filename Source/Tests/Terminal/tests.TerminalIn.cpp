#include "CommandData.h"
#include "mock.CommandInterpreter.h"
#include "mock.UartDriver.h"
#include "TerminalIn.h"
#include "gmock/gmock.h"

#include <format>
#include <numeric>
#include <vector>

using namespace ::command;
using namespace ::terminal;
using namespace ::testing;

class TerminalInTests : public ::testing::Test
{
public:
  TerminalInTests() :
    mTerminal(mUart, mCommandInterpreter)
  {
  }

protected:
  driver::MockUartDriver mUart;
  MockCommandIntepreter mCommandInterpreter;
  TerminalIn mTerminal;
};

TEST_F(TerminalInTests, start_registers_with_uart)
{
  // Given,
  ASSERT_FALSE(mUart.SetCallbackCalled);

  // When
  mTerminal.Start();

  // Then
  ASSERT_TRUE(mUart.SetCallbackCalled);
  ASSERT_EQ(&mTerminal, mUart.Callback);
}

TEST_F(TerminalInTests, start_inits_uart)
{
  // Given,
  ASSERT_FALSE(mUart.InitCalled);

  // When
  mTerminal.Start();

  // Then
  ASSERT_TRUE(mUart.InitCalled);
}

TEST_F(TerminalInTests, command_is_not_interpreted_until_task_called)
{
  // Given
  std::string command = "command\n";

  for (const auto& c : command)
    mTerminal.OnReceiveCharacter(c);

  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 0);

  // When
  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
}

TEST_F(TerminalInTests, command_is_not_interpreted_until_line_ending_received)
{
  // Given
  std::string command = "command";

  for (const auto& c : command)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 0);

  // When
  mTerminal.OnReceiveCharacter('\n');
  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
}

TEST_F(TerminalInTests, exceeding_max_command_length_discards_current_line)
{
  // Given
  std::string tooLongCommand(TerminalIn::InputQueueSize + 1, 'a');
  std::string validCommand = "command\n";

  // When
  for (const auto& c : tooLongCommand)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();

  for (const auto& c : validCommand)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  const auto commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_THAT(commandInterpreted, StartsWith(validCommand));
}

TEST_F(TerminalInTests, exceeding_max_command_length_does_not_discard_previous_line)
{
  // Given
  std::string command = "command\n";
  std::string tooLongCommand(CommandData::MaxRawInputLength + 1, 'a');

  // When
  for (const auto& c : command)
    mTerminal.OnReceiveCharacter(c);

  for (const auto& c : tooLongCommand)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  const auto commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_THAT(commandInterpreted, StartsWith(command));
}

TEST_F(TerminalInTests, Non_printable_characters_are_ignored)
{
  // Given:
  std::vector<char> nonPrintableChars {0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 12, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 127};

  std::array<char, 256 - 128> extendedAscii {};
  std::iota(extendedAscii.begin(), extendedAscii.end(), 128);

  for (const auto c : extendedAscii)
    nonPrintableChars.push_back(c);

  std::string commandFirstPart = "com";
  std::string commandSecondPart = "mand\n";
  std::string fullCommand = commandFirstPart + commandSecondPart;

  // When
  for (const auto& nonPrintableChar : nonPrintableChars)
  {
    for (const auto& c : commandFirstPart)
      mTerminal.OnReceiveCharacter(c);

    mTerminal.OnReceiveCharacter(nonPrintableChar);

    for (const auto& c : commandSecondPart)
      mTerminal.OnReceiveCharacter(c);

    mTerminal.Update();

    // Then
    ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
    const auto commandInterpreted =
      reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
    ASSERT_THAT(commandInterpreted, StartsWith(fullCommand));

    mCommandInterpreter.ResetMock();
  }
}

TEST_F(TerminalInTests, Backspace_character_removes_any_preceding_character)
{
  // Given
  std::string command = "cb\bommand\n";
  std::string expectedCommand = "command\n";

  // When
  for (const auto& ch : command)
    mTerminal.OnReceiveCharacter(ch);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  const auto commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_THAT(commandInterpreted, StartsWith(expectedCommand));
}

TEST_F(TerminalInTests, consecutive_backspaces_are_processed_correctly)
{
  // Given
  std::string input = "Hello\b\b\b\n";

  // When
  for (char c : input)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  const auto commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_THAT(commandInterpreted, StartsWith("He"));
}

TEST_F(TerminalInTests, More_backspaces_than_typed_characters_is_handled)
{
  // Given
  std::string command = "cx\b\b\b\bcommand\n";
  std::string expectedCommand = "command\n";

  // When
  for (const auto& ch : command)
    mTerminal.OnReceiveCharacter(ch);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  const auto commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_THAT(commandInterpreted, StartsWith(expectedCommand));
}

TEST_F(TerminalInTests, correct_length_passed_to_interpreter)
{
  // Given
  std::string command = "command\n";

  // When
  for (const auto& ch : command)
    mTerminal.OnReceiveCharacter(ch);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  ASSERT_EQ(command.length(), mCommandInterpreter.Length);
}

TEST_F(TerminalInTests, a_second_command_received_before_the_first_is_processed_gets_queued)
{
  // Given
  std::string firstCommand = "first command\n";
  std::string secondCommand = "second command\n";

  // When
  for (const auto& c : firstCommand)
    mTerminal.OnReceiveCharacter(c);

  for (const auto& c : secondCommand)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  auto commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_EQ(firstCommand.length(), mCommandInterpreter.Length);
  ASSERT_THAT(commandInterpreted, StrEq(firstCommand));

  mTerminal.Update();

  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 2);
  commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_THAT(commandInterpreted, StrEq(secondCommand));
}

TEST_F(TerminalInTests, multiple_commands_can_be_queued)
{
  // Given
  std::string terminalInput;
  const size_t commandCount = 10;
  std::vector<std::string> commands;

  for (size_t i = 0; i < commandCount; ++i)
  {
    const auto command = std::format("command{}\n", i);
    commands.push_back(command);
    terminalInput += command;
  }

  // When
  for (auto c : terminalInput)
    mTerminal.OnReceiveCharacter(c);

  // Then
  for (size_t i = 0; i < commandCount; ++i)
  {
    mTerminal.Update();
    ASSERT_EQ(mCommandInterpreter.InterpretCallCount, i + 1);
    ASSERT_EQ(commands[i].length(), mCommandInterpreter.Length);
    auto commandInterpreted =
      reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
    ASSERT_THAT(commandInterpreted, StrEq(commands[i]));
  }
}

TEST_F(TerminalInTests, discards_full_buffer_without_terminator)
{
  // Given
  const std::string bufferSizedStringWithoutTerminator(
    TerminalIn::InputQueueSize, 'a');
  const std::string followUpCommand = "follow-up command\n";

  // When: Buffer filled without terminator
  for (const auto& c : bufferSizedStringWithoutTerminator)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();

  // Then
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 0);

  // When: Follow-up command received
  for (const auto& c : followUpCommand)
    mTerminal.OnReceiveCharacter(c);

  mTerminal.Update();

  // Then: Follow-up command processed
  ASSERT_EQ(mCommandInterpreter.InterpretCallCount, 1);
  auto commandInterpreted =
    reinterpret_cast<const char*>(mCommandInterpreter.CommandReceived.data());
  ASSERT_EQ(followUpCommand.length(), mCommandInterpreter.Length);
  ASSERT_THAT(commandInterpreted, StrEq(followUpCommand));
}