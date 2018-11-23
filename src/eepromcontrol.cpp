#include "eepromcontrol.h"
#include <algorithm>
#include <EEPROM.h>
#include <FS.h>
#include <sstream>
#include <ArduinoJson.h>

EepromControl::EepromControl()
{
}

// void EepromControl::storeStruct(void *data_source, size_t size, int address)
// {
//   EEPROM.begin(size * 2);
//   for (size_t i = 0; i < size; i++)
//   {
//     char data = ((char *)data_source)[i];
//     EEPROM.write(i + address, data);
//   }
//   EEPROM.commit();
// }

// void EepromControl::loadStruct(void *data_dest, size_t size, int address)
// {
//   EEPROM.begin(size * 2);
//   for (size_t i = 0; i < size; i++)
//   {
//     char data = EEPROM.read(i + address);
//     ((char *)data_dest)[i] = data;
//   }
// }

void EepromControl::FormatSPIFFS()
{
  // Takes a REALLLYY LONG TIME!!! (like half a minute)
  Serial.println("Formatting SPIFFS....(this may take a minute)");
  SPIFFS.begin();
  SPIFFS.format();
  SPIFFS.end();
  Serial.println("Finished SPIFFS formatting");
}

String EepromControl::ReadFile(String fileName)
{
  SPIFFS.begin();

  String path = "/" + fileName;
  String fileData = "";

  if (SPIFFS.exists(path))
  {
    File f = SPIFFS.open(path, "r");

    if (f && f.size())
    {

      while (f.available())
      {
        fileData += char(f.read());
      }
      f.close();

    }
    else
    {

    }

  }
  SPIFFS.end();

  return fileData;
}


void EepromControl::WriteFile(String fileName, String data)
{
  SPIFFS.begin();

  String path = "/" + fileName;

  // open file for writing
  File f = SPIFFS.open(path, "w");
  if (!f) {
      Serial.println("file open failed");
  }
  f.print(data);

  f.flush();
  f.close();

  SPIFFS.end();
}



String EepromControl::ConfigToJSONString(ConfigStruct config)
{
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["PairingMode"] = config.PairingMode;
  root["MeshSSID"] = config.SSID;
  root["MeshPassword"] = config.Password;
  root["MeshPort"] = config.CommPort;

  String jsonString;
  root.printTo(jsonString);

  return jsonString;
}

ConfigStruct EepromControl::JSONStringToConfig(String jsonString)
{
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(jsonString);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
  }

  byte pairingMode = root["PairingMode"];
  String ssid = root["MeshSSID"];
  String pw = root["MeshPassword"];
  uint16_t port = root["MeshPort"];

  ConfigStruct config = {
    pairingMode,
    ssid,
    pw,
    port
  };

  return config;
}


void EepromControl::InitConfigFile()
{
  ConfigStruct initData = {
      0,
      "WemosMesh",
      "WemosMeshPassword",
      5555
  };

  FormatSPIFFS();
  SaveConfigFile(initData);
}


void EepromControl::SaveConfigFile(ConfigStruct config)
{
  String jsontext = ConfigToJSONString(config);
  WriteFile("config.json", jsontext);
}

ConfigStruct EepromControl::ReadConfigFile()
{
  ConfigStruct configData;
  String configDataString = ReadFile("config.json");

  // NEED: validate config file properly
  Serial.println(configDataString.c_str());
  if (strlen(configDataString.c_str()) > 0)
  {
      // configData = JSONStringToConfig(configDataString);
  }
  else
  {
      InitConfigFile();
      configDataString = ReadFile("config.json");
  }

  configData = JSONStringToConfig(configDataString);
  // NEED: error handling

  return configData;
}
