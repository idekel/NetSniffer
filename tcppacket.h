#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <netinet/tcp.h>

#include "packet.h"

class TCPPacket final : public Packet
{
public:

    TCPPacket(byte *buffer, int size);

    int getSourcePort() const;
    int getDestPort() const;

private:
    tcphdr *_tcph;
};

#endif // TCPPACKET_H
