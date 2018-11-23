#ifndef _CONFIGSTRUCT_H_
#define _CONFIGSTRUCT_H_

#include <stdint.h>
#include <string>

struct ConfigStruct
{
    byte PairingMode; //0 no pairing, 1 Client mode (connect to sth), 2 host mode (others can connect)
    String SSID;
    String Password;
    uint16_t CommPort;
};

#endif
