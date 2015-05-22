#ifndef REQUESTFACTORY_H
#define REQUESTFACTORY_H


#include <Poco/Net/HTTPRequestHandlerFactory.h>

class RequestFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    RequestFactory();

    ~RequestFactory();


    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &request) override;
};

#endif // REQUESTFACTORY_H
