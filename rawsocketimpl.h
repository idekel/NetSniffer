#ifndef RAWSOCKETIMPL_H
#define RAWSOCKETIMPL_H

#include <Poco/Net/SocketImpl.h>

class RawSocketImpl : public Poco::Net::SocketImpl
{
public:
    RawSocketImpl();
};

#endif // RAWSOCKETIMPL_H
