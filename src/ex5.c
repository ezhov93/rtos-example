#include <cmsis_os2.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

static void blinkInit();
static void blinkProcess(void *arg);
static void print(void *arg);

static const char *helloWorld = "Hello World\n";

int example5() {
  osKernelInitialize();
  blinkInit();
  osTimerId_t tim1 = osTimerNew(blinkProcess, osTimerPeriodic, NULL, NULL);
  osTimerStart(tim1, 250);
  osTimerId_t tim2 =
      osTimerNew(print, osTimerPeriodic, (void *)helloWorld, NULL);
  osTimerStart(tim2, 1000);
  osKernelStart();
  return 0;
}

void blinkInit() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void blinkProcess(void *arg) {
  static int state = 0;
  if (state == 0) {
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    state = 1;
  } else {
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
    state = 0;
  }
}

void print(void *arg) { printf("%s", (char *)arg); }