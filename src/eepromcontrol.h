#ifndef _EEPROMCONTROL_H_
#define _EEPROMCONTROL_H_

#include "common.h"
// #include <string>
#include <WString.h>

#include "configstruct.h"

class EepromControl
{
public:
    EepromControl();
    // void dataToEEPROM(WifiCredStruct data, uint32_t eepromAddress);
    // WifiCredStruct eepromToData(uint32_t eepromAddress);
    // void storeStruct(void *data_source, size_t size, int address = 0);
    // void loadStruct(void *data_dest, size_t size, int address = 0);

    void FormatSPIFFS();

    String ReadFile(String fileName);
    void WriteFile(String fileName, String data);
    ConfigStruct ReadConfigFile();
    void SaveConfigFile(ConfigStruct config);
    void InitConfigFile();

    String ConfigToJSONString(ConfigStruct config);
    ConfigStruct JSONStringToConfig(String jsonString);
};

#endif // !_EEPROMCONTROL_H_
