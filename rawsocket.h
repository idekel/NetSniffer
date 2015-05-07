#ifndef RAWSOCKET_H
#define RAWSOCKET_H

#include <Poco/Net/Socket.h>

class RawSocket : public Poco::Net::Socket
{
public:
    RawSocket();
};

#endif // RAWSOCKET_H
