#include <cmsis_os2.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

static void blinkThread(void *arg);
static void delayThread(void *arg);

static osSemaphoreId_t semId;

int example3() {
  osKernelInitialize();
  osThreadNew(blinkThread, NULL, NULL);
  osThreadNew(delayThread, NULL, NULL);

  semId = osSemaphoreNew(1, 1, NULL);
  osSemaphoreAcquire(semId, osWaitForever);
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

  int state = 0;
  for (;;) {
    osSemaphoreAcquire(semId, osWaitForever);
    if (state == 0) {
      GPIO_ResetBits(GPIOA, GPIO_Pin_0);
      state = 1;
    } else {
      GPIO_SetBits(GPIOA, GPIO_Pin_0);
      state = 0;
    }
  }
  osThreadTerminate(NULL);
}

void delayThread(void *arg) {
  for (;;) {
    osDelay(500);
    osSemaphoreRelease(semId);
  }
  osThreadTerminate(NULL);
}