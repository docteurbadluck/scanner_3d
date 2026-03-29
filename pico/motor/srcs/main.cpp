#include <stdio.h>
#include <pico/stdlib.h>

int main()
{
    stdio_init_all();
    gpio_init(25);
    while (1)
    {
        printf("hello world \n");
        sleep_ms(100);

    }

    return 0;
}
