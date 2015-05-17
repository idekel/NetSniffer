#include "tcppacket.h"

using namespace std;

TCPPacket::TCPPacket(byte *buffer, int size) : Packet(buffer, size)
{
    _tcph = reinterpret_cast<tcphdr*>(buffer + getIPSize() + sizeof(ethhdr));
}


int TCPPacket::getDestPort() const
{
    return ntohs(_tcph->dest);
}


int TCPPacket::getSourcePort() const
{
    return ntohs(_tcph->source);
}
