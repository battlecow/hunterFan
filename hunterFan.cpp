#include "hunterFan.h"

#ifdef RaspberryPi
// PROGMEM and _P functions are for AVR based microprocessors,
// so we must normalize these for the ARM processor:
#define PROGMEM
#define memcpy_P(dest, src, num) memcpy((dest), (src), (num))
#endif

#if defined(ESP8266)
// interrupt handler and related code must be in RAM on ESP8266,
// according to issue #46.
#define RECEIVE_ATTR ICACHE_RAM_ATTR
#define VAR_ISR_ATTR
#elif defined(ESP32)
#define RECEIVE_ATTR IRAM_ATTR
#define VAR_ISR_ATTR DRAM_ATTR
#else
#define RECEIVE_ATTR
#define VAR_ISR_ATTR
#endif

#if defined(ESP8266) || defined(ESP32)
static const VAR_ISR_ATTR HunterFan::Protocol proto = {
#else
static const HunterFan::Protocol PROGMEM proto = {
#endif
    400, {1, 2}, {2, 1}, {1, 2}};

/**
 * Array of Fan Addresses 
 */
#if defined(ESP8266) || defined(ESP32)
static const VAR_ISR_ATTR HunterFan::Address addresses[] = {
#else
static const HunterFan::Address PROGMEM addresses[] = {
#endif
    {"0011001001001111011010000001000000001110"},
    {"0010000101001110001100101011011111000100"},
    {"0001000001101010110111001111001100110010"}};

/**
 * Defined array of fan command speeds
 */
#if defined(ESP8266) || defined(ESP32)
static const VAR_ISR_ATTR HunterFan::Control controls[] = {
#else
static const HunterFan::Control PROGMEM controls[] = {
#endif
    {"off", "01111110111010000001000101"},
    {"low", "01111111111010000000000101"},
    {"med", "01111110111110000001000001"},
    {"high", "01111101111110000010000001"}};

static const int numControls = 4;

HunterFan::HunterFan()
{
    this->nTransmitterPin = -1;
    this->setRepeatTransmit(1);
    this->setProtocol();
}

/**
  * Sets the protocol to send.
  */
void HunterFan::setProtocol()
{
    this->protocol = proto;
}

/**
  * Sets pulse length in microseconds
  */
void HunterFan::setPulseLength(int nPulseLength)
{
    this->protocol.pulseLength = nPulseLength;
}

/**
 * Sets Repeat Transmits
 * 
 * @param nRepeatTransmit Set the number of times to repeat the message
 */
void HunterFan::setRepeatTransmit(int nRepeatTransmit)
{
    this->nRepeatTransmit = nRepeatTransmit;
}

/**
 * Enable transmissions
 *
 * @param nTransmitterPin    Arduino Pin to which the sender is connected to
 */
void HunterFan::enableTransmit(int nTransmitterPin)
{
    this->nTransmitterPin = nTransmitterPin;
    pinMode(this->nTransmitterPin, OUTPUT);
}

/**
 * Set the fan address
 * 
 * @param nAddress  Set the fan address
 */
void HunterFan::setAddress(int nAddress)
{
    this->address = addresses[nAddress];
}

/**
 * Loop through the array to get the control code for a specified speed name
 * 
 * @param speedName Human readable speed name (off, low, med, high)
 * @return code
 */
const char *HunterFan::getSpeedCode(const char *speedName)
{
    for (int i = 0; i < numControls; i++)
    {
        if (*controls[i].name == *speedName)
            return controls[i].code;
    }
}

/**
 * Transmit code based on address and speed name
 * 
 * @param speedName Human readable speed name (off, low, med, high)
 */
void HunterFan::sendNamedSpeed(const char *speedName)
{
    const char *speedCode = this->getSpeedCode(speedName);
    this->send(this->address.address, speedCode);
}

/**
 * @param code Binary code to send
 */
void HunterFan::send(const char *address, const char *code)
{
    const char *one = "1";
    int length = (unsigned)strlen(code);
    int addressLength = (unsigned)strlen(address);
    for (int nRepeat = 0; nRepeat < nRepeatTransmit; nRepeat++)
    {
        for (int i = 11; i >= 0; i--)
        {
            this->transmit(protocol.syncFactor);
        }
        // Delay between sync code (12 - 1s)
        delayMicroseconds(5000);
        // Send the address part first
        for (int i = 0; i <= addressLength - 1; i++)
        {
            if (address[i] == one[0])
                this->transmit(protocol.one);
            else
                this->transmit(protocol.zero);
        }
        // Send the command code
        for (int i = 0; i <= length - 1; i++)
        {
            if (code[i] == one[0])
                this->transmit(protocol.one);
            else
                this->transmit(protocol.zero);
        }
        // Delay between repeat codes
        delayMicroseconds(26000);
    }
}

/**
 * Transmit a single high-low pulse.
 */
void HunterFan::transmit(HighLow pulses)
{
    uint8_t firstLogicLevel = HIGH;
    uint8_t secondLogicLevel = LOW;

    digitalWrite(this->nTransmitterPin, firstLogicLevel);
    delayMicroseconds(this->protocol.pulseLength * pulses.high);
    digitalWrite(this->nTransmitterPin, secondLogicLevel);
    delayMicroseconds(this->protocol.pulseLength * pulses.low);
}
