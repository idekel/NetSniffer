#include "tcppacket.h"

using namespace std;

TCPPacket::TCPPacket(byte *buffer, int size) : Packet(buffer, size)
{
    _tcph = reinterpret_cast<tcphdr*>(buffer + getIPSize() + sizeof(ethhdr));
}


size_t TCPPacket::getDestPort() const
{
    return htonl(_tcph->dest);
}


size_t TCPPacket::getSourcePort() const
{
    return htonl(_tcph->source);
}
