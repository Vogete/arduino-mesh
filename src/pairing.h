#ifndef _MESH_PAIRING_H_
#define _MESH_PAIRING_H_

#include <WString.h>
#include <ArduinoJson.h>
#include "common.h"
#include "configstruct.h"


class Pairing
{
private:
    static int pairingId;
    static int GeneratePairingId();
public:
    Pairing();

    static byte pairingMode;
    static uint32_t hostDeviceId;

    static void SendWifiCred(ConfigStruct config, uint32_t destination);
    static void SendWifiCredRequest(int _pairingId, uint32_t _destination);
    static void StoreWifiCred(String ssid, String pw, uint16_t port);
    static void BroadcastPairingId();
    static void ExitPairing();

    static void HandleMessages(uint32_t from, String &msg);
    static void ClientMessageHandling(uint32_t from, String stringJsonData);
    static void HostMessageHandling(uint32_t from, String stringJsonData);
};

#endif // !_MESH_PAIRING_H_
