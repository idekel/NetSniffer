#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <netinet/tcp.h>

#include "packet.h"

class TCPPacket final : public Packet
{
public:

    TCPPacket(byte *buffer, int size);

    void savePacket(Poco::MongoDB::Document &doc) override;

    int getSourcePort() const;
    int getDestPort() const;

private:
    tcphdr *_tcph;
};

#endif // TCPPACKET_H
