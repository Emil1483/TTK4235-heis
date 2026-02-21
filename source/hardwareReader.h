#pragma once
#include "elevator.h"

struct HardwareReader{
    Elevator* elevator;
    void (*doReading)();
};

void doReading(struct HardwareReader* reader);

struct HardwareReader* hardwareReader_constructor();
