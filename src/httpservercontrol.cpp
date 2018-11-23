#include "httpservercontrol.h"
#include "common.h"
#include "meshcontrol.h"
#include "ledcontrol.h"

// REST Server
#include <stdio.h>
#include <ArduinoJson.h>

ESP8266WebServer HttpServerControl::http_rest_server(80);

HttpServerControl::HttpServerControl(){
}


void HttpServerControl::config_rest_server_routing()
{
    http_rest_server.on("/", HTTP_GET, []() {
        http_rest_server.send(200, "text/html",
                              "Welcome to the ESP8266 REST Web Server");
    });
    // http_rest_server.on("/mesh", HTTP_GET, getNodesInMesh);
    // http_rest_server.on("/changeLEDPattern", HTTP_GET, apiChangeLedPattern);

    // http_rest_server.on("/leds", HTTP_POST, post_put_leds);
    // http_rest_server.on("/leds", HTTP_PUT, post_put_leds);
}

// REST Server stuff
void HttpServerControl::getNodesInMesh()
{
    String meshNodes = MeshControl::getNodesInMesh();
    http_rest_server.send(200, "application/json", meshNodes.c_str());
}


void HttpServerControl::apiChangeLedPattern()
{
    LedControl::changeLEDPattern();
    MeshControl::sendMeshMessage("switch light mode");
    http_rest_server.send(200, "text/html", "Changed LED Pattern! :)");
}
