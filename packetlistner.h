#ifndef PACKETLISTNER_H
#define PACKETLISTNER_H

#include <Poco/Runnable.h>

#include "packet.h"
#include "rawsocket.h"

class PacketListner final : public Poco::Runnable
{
public:
    PacketListner();    
    ~PacketListner();

    void run() override;

    Packet::Ptr getPacket(byte *buf, int sz);

    static int BUFSIZE;

private:
    byte *_buffer;
    RawSocket _socket;

};

#endif // PACKETLISTNER_H
