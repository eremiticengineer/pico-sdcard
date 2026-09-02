#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

#include <string>

#include "ff_headers.h"
#include "ff_sddisk.h"
#include "ff_stdio.h"
#include "ff_utils.h"

extern "C" void writeToSDTask(void* pvParameters);

class SDCard {
public:
    SDCard();
    void init();
    void writeAfterInit(const std::string& data);
    void deinit();
    void writeDataToDisk();
    void write(const std::string& data);
    void write2();
    void mount();
    void unmount();
    void writeData(const std::string& data);
private:
    FF_Disk_t *pxDisk;
};
