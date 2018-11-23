#ifndef _HTTPSERVERCONTROL_H_
#define _HTTPSERVERCONTROL_H_

#include <ESP8266WebServer.h>


class HttpServerControl
{
private:

public:
    HttpServerControl();
    static void config_rest_server_routing();
    static void getNodesInMesh();
    static void apiChangeLedPattern();

    static ESP8266WebServer http_rest_server;

};



#endif // !_HTTPSERVERCONTROL_H_
