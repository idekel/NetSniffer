#ifndef PACKETFILTER_H
#define PACKETFILTER_H

#include <memory>

#include "packet.h"

class PacketFilter
{
public:

    enum {
        ICMP = 1,
        IGMP = 2,
        TCP = 6,
        UDP = 17
    };


    PacketFilter();
    ~PacketFilter();

private:
    std::shared_ptr<Packet> _packet;
};

#endif // PACKETFILTER_H
