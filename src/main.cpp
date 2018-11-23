/*******************************************************************************
********************************************************************************
IMPORTANT NOTES:
https://github.com/Coopdis/easyMesh/issues/16
ONLY version 2.3.0 of the esp8266 board package is working!

Most stable easymesh library: https://github.com/sfranzyshen/easyMesh
PainlessMesh is cool and all, but has time sync issues, and not stable

Other useful stuff:
https://github.com/BlackEdder/easyMesheD



Pin mapping for wemos D1 mini (GPIO numbers) (https://github.com/esp8266/Arduino/blob/master/variants/d1_mini/pins_arduino.h#L49-L61):
    D0 = 16
    D1 = 5
    D2 = 4
    D3 = 0
    D4 = 2 //LED_BUILTIN
    D5 = 14
    D6 = 12
    D7 = 13
    D8 = 15
    RX = 3
    TX = 1
********************************************************************************
*******************************************************************************/

#include <string>
#include <list>
#include <ArduinoJson.h>

#include "common.h"
#include "buttoncontrol.h"
#include "ledcontrol.h"
#include "meshcontrol.h"
#include "httpservercontrol.h"
#include "eepromcontrol.h"
#include "configstruct.h"
#include "pairing.h"

ButtonControl button1(15);       //D8
ButtonControl pairingButton(12); //D6

// Scheduler userScheduler; // to control your personal task

// User stub
// void sendMessage(); // Prototype so PlatformIO doesn't complain

// Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);

int Common::HTTP_REST_PORT;
int Common::WIFI_RETRY_DELAY;

int Common::NUM_LEDS;

int Common::buttonState;
int Common::lastButtonState;

byte Common::scale;

unsigned long Common::timer;
unsigned int Common::frameRate;

byte Common::ledPattern;

uint32_t Common::myNodeID;
int Common::noNodes;

byte Common::currentMode;
// ESP8266WebServer HttpServerControl::http_rest_server;

void setGlobalVariables()
{
    Common::HTTP_REST_PORT = 80;
    Common::WIFI_RETRY_DELAY = 500;

    Common::NUM_LEDS = 9;

    Common::buttonState = 0;
    Common::lastButtonState = 0;

    Common::scale = 150; // 0-255

    Common::timer = 0;
    Common::frameRate = 40;

    Common::ledPattern = 1;

    Common::myNodeID = 0;
    Common::noNodes = 1;

    // Common::configFileName = "config.json";
}

void SwitchLightMode()
{
    Serial.println("Button pressed in callback");
    LedControl::changeLEDPattern();
    MeshControl::sendMeshMessage("switch light mode");
}

void MeshMessageHandle(uint32_t from, String &msg)
{

    if (msg == "switch light mode")
    {
        SwitchLightMode();
    }

}

void SendWifiCredRequest()
{
    Pairing::SendWifiCredRequest(0, Pairing::hostDeviceId);
}

void SetupPairingMode(ConfigStruct configData)
{
    Pairing::pairingMode = configData.PairingMode;

    MeshControl::setMessageCallback(&Pairing::HandleMessages);
    MeshControl::setupMesh("MeshPairing", "MeshPairingPassword", 5555);

    // Pairing button functions:
    // host mode:
    //      short press: prepare to pair one device (generate and broadcast new pairingId)
    //      long press: reboot with original settings
    // client mode:
    //      short press: pair device
    //      long press: reboot with original settings

    if (Pairing::pairingMode == 1) {
        // Client mode
        Serial.println("Pairing mode activated in Client mode (SSID: MeshPairing, PW: MeshPairingPassword, CommPort: 5555");
        // button press setup
        // TODO: button debounce
        pairingButton.setShortButtonPressMethod(&SendWifiCredRequest);
        pairingButton.setLongButtonPressMethod(&Pairing::ExitPairing);

    } else if (Pairing::pairingMode == 2)
    {
        // Host Mode
        Serial.println("Pairing mode activated in Host mode (SSID: MeshPairing, PW: MeshPairingPassword, CommPort: 5555");
        // button press setup
        // TODO: button debounce
        pairingButton.setShortButtonPressMethod(&Pairing::BroadcastPairingId);
        pairingButton.setLongButtonPressMethod(&Pairing::ExitPairing);
    }
}


void RebootIntoPairing_Client()
{
    EepromControl eepromControl;
    ConfigStruct config = eepromControl.ReadConfigFile();

    config.PairingMode = 1;

    eepromControl.SaveConfigFile(config);
    Common::RebootDevice();
}

void RebootIntoPairing_Host()
{
    EepromControl eepromControl;
    ConfigStruct config = eepromControl.ReadConfigFile();

    config.PairingMode = 2;

    eepromControl.SaveConfigFile(config);
    Common::RebootDevice();
}

void SetupMeshMode(ConfigStruct configData)
{
    MeshControl::setMessageCallback(&MeshMessageHandle);
    MeshControl::setupMesh(configData.SSID, configData.Password, configData.CommPort);
    // MeshControl::setupMesh("zfrWemosMesh", "potatochips3214");

    // HttpServerControl::http_rest_server(Common::HTTP_REST_PORT);
    // rest api
    HttpServerControl::config_rest_server_routing();
    HttpServerControl::http_rest_server.begin();
    Serial.println("HTTP REST Server Started");

    // void (*lightmodeSwitchPtr)() = &SwitchLightMode;
    button1.setShortButtonPressMethod(&SwitchLightMode);

    pairingButton.setShortButtonPressMethod(&RebootIntoPairing_Client);
    pairingButton.setLongButtonPressMethod(&RebootIntoPairing_Host);
}

void LoopMeshMode()
{
    HttpServerControl::http_rest_server.handleClient();

    if (millis() - Common::timer > 1000 / Common::frameRate)
    {
        Common::timer = millis();
        LedControl::showLEDPattern();
        LedControl::loopLed();
    }
}

void LoopPairingMode()
{

}

void setup()
{
    setGlobalVariables();
    LedControl::setupLed();
    delay(1000);
    Serial.begin(115200);

    EepromControl eepromControl;

    // eepromControl.InitConfigFile();

    ConfigStruct configData = eepromControl.ReadConfigFile();

    Common::currentMode = configData.PairingMode;
    Serial.println(Common::currentMode);

    if (Common::currentMode == 0)
    {
        SetupMeshMode(configData);
    }
    else
    {
        SetupPairingMode(configData);
    }
}

void loop()
{
    MeshControl::updateMesh();
    button1.handdleButtonPress();
    pairingButton.handdleButtonPress();

    if (Common::currentMode == 0) {
        LoopMeshMode();
    } else
    {
        LoopPairingMode();
    }

}
