# HunterFan
Hunter fan speed control library

# Purpose
Hunter fan control library for 433/434MHz, they use a different protocol than everyone else so decoded the signal via piscope and made this library to only transmit codes.
Hunter fan remote model number 99122 [FCC ID IN2TX41](https://fccid.io/IN2TX43)

# Acknoledgements
Shamelessly copied a good deal of code from [RC-Switch](https://github.com/sui77/rc-switch) to assist in setting everything up with the target of ESP8266 in mind.
The PiScope tool and pigpio library was instrumental in reading and decoding the sent signals from the remote [PiScope](http://abyz.me.uk/rpi/pigpio/piscope.html)
