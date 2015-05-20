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

    bool isRunning()
    {
        return _running;
    }

    Packet::Ptr getPacket(byte *buf, int sz);

    static int BUFSIZE;

private:
    byte *_buffer;
    bool _running;
    RawSocket _socket;

};

#endif // PACKETLISTNER_H
