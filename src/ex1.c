#include <cmsis_os2.h>
#include <stdio.h>

static void threadApp(void *arg);

int example1() {
  osKernelInitialize();

  osThreadAttr_t threadAppAttr = {0};
  threadAppAttr.name = "App";
  threadAppAttr.priority = osPriorityLow;
  threadAppAttr.stack_size = 128;
  osThreadId_t threadAppId = osThreadNew(threadApp, NULL, &threadAppAttr);
  osKernelStart();
  return 0;
}

void threadApp(void *arg) {
  (void)arg;
  volatile int i = 0;
  for (;;) {
    printf("%d", i);
    i++;
    osDelay(1000);
  }
  osThreadTerminate(NULL);
}