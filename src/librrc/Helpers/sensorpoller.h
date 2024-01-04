#pragma once
#include <stdint.h>
#include <librrc/Interface/networksensor.h>

class SensorPoller
{

public:
    SensorPoller(uint16_t poll_interval, NetworkSensor *networksensor) : _poll_interval(poll_interval),
                                                                         _networksensor(networksensor){};

    void setup();
    void update();
    float getVal();
    bool newdata;

private:
    const uint16_t _poll_interval;
    NetworkSensor *_networksensor;

    float sensorvalue;

    // const NetworkSensorState* sensorState;

    uint64_t last_time;
    uint64_t prevNewUpdateTime = 0;
    uint64_t networkTimeout = 1000;
};