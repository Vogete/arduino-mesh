#include "common.h"
#include "meshcontrol.h"
#include "ledcontrol.h"

// #define MESH_PREFIX "zfrWemosMesh"
// #define MESH_PASSWORD "potatochips3214"
// #define MESH_PORT 5555
#define MESH_CHANNEL 1
#define HOSTNAME "WEMOS-MESH"
#define STATION_SSID "Kukucs Guest"
#define STATION_PASSWORD "kukucs357"

easyMesh MeshControl::mesh;

// list of all the nodes in the mesh (excluding the current one)
std::list<uint32_t> MeshControl::meshNodes;

void (*MeshControl::messageReceiveCallback)(uint32_t from, String &msg);

MeshControl::MeshControl()
{
}

void MeshControl::setMessageCallback(void (*callbackFunc)(uint32_t from, String &msg))
{
    messageReceiveCallback = *callbackFunc;
}

void MeshControl::updateMesh()
{
    mesh.update();
}

void MeshControl::receivedCallback(uint32_t from, String &msg)
{
    Serial.printf("Message received from %u msg= %s\n", from, msg.c_str());

    (*messageReceiveCallback)(from, msg);
}

void MeshControl::newConnectionCallback(bool adopt)
{
    Serial.printf("startHere: New Connection, adopt = %d\n", adopt);
    Common::noNodes = mesh.connectionCount() + 1;
}

void MeshControl::sendMeshMessage(String message, uint32_t destination)
{
    // String msg = "Hello from node ";
    // msg += mesh.getNodeId();
    // taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));

    if (destination == 0)
    {
        mesh.sendBroadcast(message);
        Serial.printf("mesasge broadcasted: %s", message.c_str());
    }
    else
    {
        mesh.sendSingle(destination, message);
        Serial.printf("mesasge sent to %u : %s", destination, message.c_str());
    }
}

String MeshControl::getNodesInMesh()
{
    // return mesh.subConnectionJson();
}

void MeshControl::setupMesh(String ssid, String password, uint16_t port)
{
    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP); // set before init() so that you can see startup messages

    mesh.init(ssid, password, port);
    mesh.setReceiveCallback(&receivedCallback);
    mesh.setNewConnectionCallback(&newConnectionCallback);

    // mesh.onChangedConnections(&changedConnectionCallback);
    // mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    Common::myNodeID = mesh.getChipId();
    Serial.printf("MyNodeID: %u\n", Common::myNodeID);

    if (Common::myNodeID == 3943449884)
    {
        // mesh.setRoot(); //maybe helps with stabilization?
        // mesh.stationManual(STATION_SSID, STATION_PASSWORD);
        // mesh.setHostname(HOSTNAME);
        // Serial.printf("Connected to WiFi network %s\n", STATION_SSID);
    }
    else
    {
        // mesh.setContainsRoot(); //maybe helps with stabilization?
    }

    // userScheduler.addTask(taskSendMessage);
    // taskSendMessage.enable();
}
