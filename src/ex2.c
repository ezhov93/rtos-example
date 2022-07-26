#include <cmsis_os2.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

static void blinkThread(void *arg);
static void printThread(void *arg);

static osMessageQueueId_t msgQ;

int example2() {
  osKernelInitialize();
  osThreadId_t blinkId = osThreadNew(blinkThread, NULL, NULL);
  osThreadId_t printId = osThreadNew(printThread, NULL, NULL);

  msgQ = osMessageQueueNew(5, sizeof(uint16_t), NULL);
  osKernelStart();
  return 0;
}

void blinkThread(void *arg) {
  __disable_irq();
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  __enable_irq();

  uint16_t delay = 500;
  osStatus_t status;
  int state = 0;
  for (;;) {
    status = osMessageQueueGet(msgQ, &delay, NULL, 0);
    if (status == osOK)
      __NOP();
    if (state == 0) {
      GPIO_ResetBits(GPIOA, GPIO_Pin_0);
      state = 1;
    } else {
      GPIO_SetBits(GPIOA, GPIO_Pin_0);
      state = 0;
    }
    osDelay(delay);
  }
  osThreadTerminate(NULL);
}

void printThread(void *arg) {
  uint16_t delay;
  for (;;) {
    printf("Specify the PA1 LED blink period: ");
    scanf("%hu", &delay);
    printf("\r\nSpecified period: %hu\n\r", delay);
    osMessageQueuePut(msgQ, &delay, NULL, osWaitForever);
  }
  osThreadTerminate(NULL);
}