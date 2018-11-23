#ifndef _MESHCONTROL_H_
#define _MESHCONTROL_H_

#include <string>
#include <easyMesh.h>
#include <list>

class MeshControl
{
private:
    static std::list<uint32_t> meshNodes;
    static easyMesh mesh;
    static void (*messageReceiveCallback)(uint32_t from, String &msg);

    static void receivedCallback(uint32_t from, String &msg);
    static void newConnectionCallback(bool adopt);
public:
    MeshControl();

    static void setMessageCallback(void (*callbackFunc)(uint32_t from, String &msg));
    static void setupMesh(String ssid, String password, uint16_t port = 5555);
    static void updateMesh();
    static void sendMeshMessage(String message, uint32_t destination = 0);
    static String getNodesInMesh();

};



#endif // !_MESHCONTROL_H_
