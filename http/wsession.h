#ifndef WSESSION_H
#define WSESSION_H


#include <Poco/Net/WebSocket.h>
#include <Poco/JSON/Object.h>

class WSession : public Poco::Net::WebSocket
{
public:
    WSession(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
    ~WSession();

    void sendJson(Poco::JSON::Object &obj);
};

#endif // WSESSION_H
