#include "FreeRTOS.h" /* Must come first. */
#include "task.h"

#include <stdio.h>
#include "pico/stdlib.h"

#include "sdcard.h"

// Standard Task priority
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)

void writeToSDCardTask(void* pvParameters) {
    SDCard *pSDCard = static_cast<SDCard *>(pvParameters);
    pSDCard->init();

    while (true)
    {
        pSDCard->writeAfterInit("test for sdcard");

        printf("written to sdcard\n");

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

int main(void) {
  stdio_init_all();

  sleep_ms(5000);

  SDCard sdcard;
  xTaskCreate(writeToSDCardTask, "WriteToSDCardTask", 4096, (void*)&sdcard, TASK_PRIORITY, NULL);

  vTaskStartScheduler();

  while(1) {};
}
