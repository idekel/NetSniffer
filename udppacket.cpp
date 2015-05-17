#include "udppacket.h"

UDPPacket::UDPPacket(byte *buffer, int sz) : Packet(buffer, sz)
{
    _udph = (struct udphdr*)(buffer + getIPSize()  + sizeof(struct ethhdr));
}

UDPPacket::~UDPPacket()
{

}


int UDPPacket::getSourcePort() const
{
    return _udph->uh_sport;
}

int UDPPacket::getDestPort() const
{
    return _udph->uh_sport;
}
