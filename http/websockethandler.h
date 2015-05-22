#ifndef WEBSOCKETHANDLER_H
#define WEBSOCKETHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>


class WebSocketHandler : public Poco::Net::HTTPRequestHandler
{
public:
    WebSocketHandler();
    ~WebSocketHandler();

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // WEBSOCKETHANDLER_H
