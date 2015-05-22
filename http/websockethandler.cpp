#include "websockethandler.h"
#include "wsession.h"

using namespace std;
using namespace Poco;
using namespace Net;

WebSocketHandler::WebSocketHandler()
{

}

WebSocketHandler::~WebSocketHandler()
{

}


void WebSocketHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    WSession ws(request, response);
}

