#include "mock.UartDriver.h"
#include "TerminalOut.h"
#include "gmock/gmock.h"

using namespace ::testing;
using namespace ::terminal;

class TerminalOutTests : public ::testing::Test
{
public:
  TerminalOutTests() :
    mTerminalOut(mUart)
  {
  }

protected:
  driver::MockUartDriver mUart;
  TerminalOut mTerminalOut;
};

TEST_F(TerminalOutTests, text_out_writes_text_to_uart)
{
  // Given
  std::string text = "test\n";

  // When
  mTerminalOut.TextOut(text.c_str());

  // Then
  ASSERT_TRUE(mUart.TransmitCalled);
  ASSERT_EQ(text.length(), mUart.TransmitSize);

  const auto actualDataOut = reinterpret_cast<const char*>(mUart.TxData.data());
  ASSERT_THAT(actualDataOut, StartsWith(text));
}

TEST_F(TerminalOutTests, TextOut_accepts_format_string_and_arguments)
{
  // Given
  const size_t aSize = 5;
  const int32_t anInt = -1;
  const size_t anAddress = 0xCAFEBEEF;
  const double aDouble = 123.4;
  const char* aString = "A..Z";
  const char* formatString =
    "aSize:%zu, anInt:%i, anAddress:0x%08zX, aFloat: %.3f, aString:%s";
  const std::string expectedOutput =
    "aSize:5, anInt:-1, anAddress:0xCAFEBEEF, aFloat: 123.400, aString:A..Z";

  // When
  mTerminalOut.TextOut(formatString, aSize, anInt, anAddress, aDouble, aString);

  // Then
  const auto actualDataOut = reinterpret_cast<const char*>(mUart.TxData.data());
  ASSERT_THAT(actualDataOut, StartsWith(expectedOutput));
}
