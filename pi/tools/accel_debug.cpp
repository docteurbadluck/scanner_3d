#include "4_drivers/Accelerometer_Driver/Accelerometer_Driver.hpp"
#include <cstdio>
#include <unistd.h>

namespace
{
void printSample(int i, float dev1, float dev2)
{
    printf("sample %2d | acc1 dev=%.4f g | acc2 dev=%.4f g\n", i, dev1, dev2);
}

void runSamples(Accelerometer_Driver &acc1, Accelerometer_Driver &acc2, int count)
{
    for (int i = 0; i < count; ++i)
    {
        printSample(i, acc1.readMagnitude(), acc2.readMagnitude());
        usleep(200 * 1000);
    }
}
}

int main()
{
    Accelerometer_Driver acc1({ "/dev/i2c-1", 0x68 });
    Accelerometer_Driver acc2({ "/dev/i2c-1", 0x69 });
    printf("threshold for isStable() = 0.05 g, reading every 200ms, keep the rig still\n");
    runSamples(acc1, acc2, 50);
}
