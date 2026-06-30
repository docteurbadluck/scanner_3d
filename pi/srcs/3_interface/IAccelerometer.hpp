#pragma once

class IAccelerometer
{
public:
    virtual ~IAccelerometer() = default;
    virtual float readMagnitude() = 0;
};
