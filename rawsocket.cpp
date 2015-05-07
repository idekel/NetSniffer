#include "rawsocket.h"
#include "rawsocketimpl.h"

using namespace Poco::Net;

RawSocket::RawSocket() : Socket(new RawSocketImpl)
{

}
