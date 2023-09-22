#include "UartDriver.h"
#include "usart.h"

using namespace ::driver;
static ITerminalReceiveCharacterCallback* sCallback = nullptr;

static constexpr uint32_t TimeoutMs = 10;

void UartDriver::Init()
{
  // USART2 interrupt init
  HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

  // Receive Data register not empty interrupt
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  // Error interrupt(Frame error, noise error, overrun error)
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);
  // Parity Error interrupt
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_PE);
}

void UartDriver::Transmit(const uint8_t* txData, size_t size) const
{
  for (size_t i = 0; i < size; i++)
  {
    while ((huart2.Instance->SR & USART_SR_TXE) == 0)
      ;
    huart2.Instance->DR = *txData;
    txData++;
  }

  while ((huart2.Instance->SR & USART_SR_TC) == 0)
    ;
}

void UartDriver::SetCallback(ITerminalReceiveCharacterCallback* callback) const
{
  sCallback = callback;
}

extern "C" void USART2_IRQHandler(void)
{
  // Overrun, Noise, Framing, Parity error occurred
  if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_ORE)
    || __HAL_UART_GET_FLAG(&huart2, UART_FLAG_NE)
    || __HAL_UART_GET_FLAG(&huart2, UART_FLAG_FE)
    || __HAL_UART_GET_FLAG(&huart2, UART_FLAG_PE))
  {
    // macro will clear any of the above flags
    __HAL_UART_CLEAR_PEFLAG(&huart2);
    return;
  }

  if (sCallback == nullptr)
    return;

  bool byteIsReceived = ((huart2.Instance->SR & UART_FLAG_RXNE) != 0);
  bool interruptIsByteReceived = ((huart2.Instance->CR1 & UART_IT_RXNE) != 0);
  if (!byteIsReceived || !interruptIsByteReceived)
    return;

  uint8_t byteReceived = (uint8_t)(huart2.Instance->DR & 0xFF);
  sCallback->OnReceiveCharacter(static_cast<char>(byteReceived));
}
