#include "pairing.h"
#include "meshcontrol.h"
#include "eepromcontrol.h"

//  MessageType values:
//  1: PairingId
//  2: WifiCredRequest
//  3: WifiCredentials
//  4: CredConfirm

byte Pairing::pairingMode = 1;
int Pairing::pairingId;
uint32_t Pairing::hostDeviceId = 0;

Pairing::Pairing()
{
    // input param:    byte _pairingMode

    // pairingMode = _pairingMode;
    // pairingId = GeneratePairingId();
}

void Pairing::SendWifiCred(ConfigStruct config, uint32_t destination)
{
    // Sends the wifi credentials to the node that requested it
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["MessageType"] = 3; //WifiCredentials
    root["MeshSSID"] = config.SSID;
    root["MeshPassword"] = config.Password;
    root["MeshPort"] = config.CommPort;

    String jsonString;
    root.printTo(jsonString);

    // TODO: needs to be changed back to send it to the right node
    MeshControl::sendMeshMessage(jsonString);
    // MeshControl::sendMeshMessage(jsonString, destination);
}

void Pairing::SendWifiCredRequest(int _pairingId, uint32_t _destination)
{
    Serial.println("Sending Wifi Credential request");
    // Sends a request to get the wifi credentials

    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["MessageType"] = 2; // WifiCredRequest
    root["ClientId"] = Common::myNodeID;
    root["PairingId"] = _pairingId;

    String jsonString;
    root.printTo(jsonString);

    MeshControl::sendMeshMessage(jsonString, _destination);
}

void Pairing::StoreWifiCred(String ssid, String pw, uint16_t port)
{
    // Stores the wifi credentials
    EepromControl eepromControl;

    ConfigStruct config = {
        0,
        ssid,
        pw,
        port
    };

    eepromControl.SaveConfigFile(config);
}

void Pairing::BroadcastPairingId()
{
    // Broadcast pairing ID for one time usage
    pairingId = GeneratePairingId();

    Serial.println("Broadcasting pairing ID");

    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["MessageType"] = 1; //PairingId
    root["HostId"] = Common::myNodeID;
    root["PairingId"] = pairingId;

    String jsonString;
    root.printTo(jsonString);

    MeshControl::sendMeshMessage(jsonString);

}

void Pairing::HandleMessages(uint32_t from, String &msg)
{
    Serial.printf("Incoming message: %s", msg.c_str());
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(msg);

    // Test if parsing succeeds.
    if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
    }

    if (pairingMode == 1) {
        // Client mode
        ClientMessageHandling(from, msg);

    } else if (pairingMode == 2)
    {
        // Host Mode
        HostMessageHandling(from, msg);
    }


}


void Pairing::ClientMessageHandling(uint32_t from, String stringJsonData)
{
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& jsonData = jsonBuffer.parseObject(stringJsonData);

    byte messageType = jsonData["MessageType"];

    switch (messageType)
    {
        case 1:
            pairingId = jsonData["PairingId"];
            hostDeviceId = from;
            SendWifiCredRequest(pairingId, hostDeviceId);
            break;

        case 3:
            StoreWifiCred(jsonData["MeshSSID"], jsonData["MeshPassword"], jsonData["MeshPort"]);
            ExitPairing();
            break;

        default:
            break;
    }

}

void Pairing::HostMessageHandling(uint32_t from, String stringJsonData)
{
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& jsonData = jsonBuffer.parseObject(stringJsonData);

    byte messageType = jsonData["MessageType"];


    switch (messageType)
    {
        case 4: {
            ExitPairing();
            break;
        }
        case 2: {
            // NEED: pairing ID for a bit better security?
            EepromControl eepromControl;
            ConfigStruct config;
            config = eepromControl.ReadConfigFile();
            SendWifiCred(config, from);
            // ExitPairing(); // if this is added, then the message won't be sent properly
            break;
        }
        default: {
            break;
        }
    }
}


int Pairing::GeneratePairingId()
{
    int rndNumber = rand();
    return rndNumber;
}


void Pairing::ExitPairing()
{
    Serial.println("Exit pairing and rebooting");
    EepromControl eepromControl;
    ConfigStruct config = eepromControl.ReadConfigFile();

    config.PairingMode = 0;

    eepromControl.SaveConfigFile(config);
    Common::RebootDevice();
}


