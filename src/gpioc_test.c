#include <stdio.h>
#include <unistd.h>

#include "gpioc.h"

int main(void)
{
    gpioc_open();

    gpioc_setPinOutput(12);

    gpioc_setPinOn(12);
    
    sleep(1);

    gpioc_setPinOff(12);

    gpioc_close();

    return 0;
}
