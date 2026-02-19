#pragma once
#include "elevator.h"

struct HardwareReader{
    struct Elevator elevator;
    void (*doReading)();
};

void doReading();

struct HardwareReader hardwareReader_constructor();
