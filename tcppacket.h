#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <netinet/tcp.h>

#include "packet.h"

class TCPPacket final : public Packet
{
public:

    TCPPacket(byte *buffer, int size);

    size_t getSourcePort() const;
    size_t getDestPort() const;

private:
    tcphdr *_tcph;
};

#endif // TCPPACKET_H
