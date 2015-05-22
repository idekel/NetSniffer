#ifndef DEFAULTHANDLER_H
#define DEFAULTHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>

class DefaultHandler : public Poco::Net::HTTPRequestHandler
{
public:
    DefaultHandler();

    ~DefaultHandler();

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DEFAULTHANDLER_H
