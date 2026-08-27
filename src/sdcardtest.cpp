#include "FreeRTOS.h" /* Must come first. */
#include "task.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "cbsd/cbsd.h"

// Standard Task priority
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)

void writeToSDCardTask(void* pvParameters) {
    Codebrane::CBSD *pCBSD = static_cast<Codebrane::CBSD *>(pvParameters);
    pCBSD->init();

    while (true)
    {
        pCBSD->writeAfterInit("test for sdcard");

        printf("written to sdcard\n");

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

int main(void) {
  stdio_init_all();

  sleep_ms(5000);

  Codebrane::CBSD cbsd;
  xTaskCreate(writeToSDCardTask, "WriteToSDCardTask", 4096, (void*)&cbsd, TASK_PRIORITY, NULL);

  vTaskStartScheduler();

  while(1) {};
}
