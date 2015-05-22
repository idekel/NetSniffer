#include "requestfactory.h"
#include "websockethandler.h"
#include "defaulthandler.h"


#include <Poco/Net/HTTPServerRequest.h>

using namespace std;
using namespace Poco;
using namespace Net;


RequestFactory::RequestFactory()
{

}

RequestFactory::~RequestFactory()
{

}



HTTPRequestHandler* RequestFactory::createRequestHandler(const HTTPServerRequest &request)
{
    if (request.find("Upgrade") != request.end())
    {
        return new WebSocketHandler;
    }

    return new DefaultHandler;
}
