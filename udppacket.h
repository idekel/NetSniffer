#ifndef UDPPACKET_H
#define UDPPACKET_H

#include <netinet/udp.h>

#include "packet.h"


class UDPPacket final : public Packet
{
public:

    UDPPacket(byte *buffer, int sz);

    ~UDPPacket();

    int getSourcePort() const;
    int getDestPort() const;

private:
    struct udphdr* _udph;
};

#endif // UDPPACKET_H
