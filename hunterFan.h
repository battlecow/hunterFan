#ifndef _HunterFan_h
#define _HunterFan_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(RPI) // Raspberry Pi
#define RaspberryPi

// Include libraries for RPi:
#include <string.h> /* memcpy */
#include <stdlib.h> /* abs */
#include <wiringPi.h>
#elif defined(SPARK)
#include "application.h"
#else
#include "WProgram.h"
#endif

#include <stdint.h>

class HunterFan
{

public:
    HunterFan();

    void setProtocol();
    void setPulseLength(int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);
    void enableTransmit(int nTransmitterPin);
    void send(const char *address, const char *code);
    void setAddress(int nAddress);

    const char *getSpeedCode(const char *speedName);

    void sendNamedSpeed(const char *speedName);

    /**
     * Description of a single pule, which consists of a high signal
     * whose duration is "high" times the base pulse length, followed
     * by a low signal lasting "low" times the base pulse length.
     * Thus, the pulse overall lasts (high+low)*pulseLength
     */
    struct HighLow
    {
        uint8_t high;
        uint8_t low;
    };

    /**
     * A "protocol" describes how zero and one bits are encoded into high/low
     * pulses.
     */
    struct Protocol
    {
        /** base pulse length in microseconds, e.g. 350 */
        uint16_t pulseLength;

        HighLow syncFactor;
        HighLow zero;
        HighLow one;
    };

    struct Address
    {
        const char *address;
    };

    struct Control
    {
        const char *name;
        const char *code;
    };

private:
    void transmit(HighLow pulses);

    int nTransmitterPin;
    int nRepeatTransmit;

    Address address;

    Protocol protocol;
};

#endif
