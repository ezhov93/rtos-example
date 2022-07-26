#include "FreeRTOS.h"
#include <stdlib.h>

#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

void retargetInit() {
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |
                             RCC_APB2Periph_AFIO,
                         ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);
}

void *malloc(size_t size) { return pvPortMalloc(size); }

void free(void *ptr) { vPortFree(ptr); }

int fputc(int ch, FILE *f) {
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    __NOP();
  USART_SendData(USART1, (char)ch);
  return ch;
}

int fgetc(FILE *f) {
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    __NOP();
  char ch = USART_ReceiveData(USART1);
  USART_SendData(USART1, (char)ch);
  return ch;
}

int ferror(FILE *f) { return 0; }