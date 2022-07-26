#include <cmsis_os2.h>
#include <stdio.h>

#define USE_MUTEX 1

static void print(void *arg);

static const char *Hello = "Hello\n";
static const char *Bonjur = "Bonjur\n";

static osMutexId_t mutex;

int example6() {
  osKernelInitialize();
  mutex = osMutexNew(NULL);
  osThreadNew(print, (void *)Hello, NULL);
  osThreadNew(print, (void *)Bonjur, NULL);
  osKernelStart();
  return 0;
}

void print(void *arg) {
  for (;;) {
#if USE_MUTEX == 1
    osMutexAcquire(mutex, osWaitForever);
    printf("%s", (char *)arg);
    osMutexRelease(mutex);
#else
    printf("%s", (char *)arg);
#endif
  }
}