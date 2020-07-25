#include <hunterFan.h>

int main(int argc, char *argv[])
{
    HunterFan fan = HunterFan();
    // For a raspberry pi relying on wiringPi
    if (wiringPiSetupGpio() == -1)
        return 1;
    // Use gpio pin 17
    fan.enableTransmit(17);
    // Repeat transmission
    fan.setRepeatTransmit(2);

    // Set the address to the first in the array
    fan.setAddress(0);
    // Set a speed by name (off, low, med, high)
    fan.sendNamedSpeed("low");
}