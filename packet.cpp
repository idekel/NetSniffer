#include "packet.h"

#include <arpa/inet.h>

#include <cstring>
#include <cstdio>

#include <Poco/Exception.h>
#include <Poco/JSON/Object.h>

using namespace std;
using namespace Poco;
using namespace MongoDB;
using namespace JSON;

Packet::Packet(byte *buffer, int sz) : _size(sz)
{
    if (_size <= 0 || !buffer)
    {
        throw Poco::InvalidArgumentException("invalid size or empty buffer");
    }

    if (_size < (sizeof(ethhdr) + sizeof(iphdr)))
    {
        throw Poco::InvalidArgumentException("invalid size. packet must content Ethernet headers plus IP headers");
    }

    _buffer = new unsigned char[_size];

    memcpy(_buffer, buffer, _size);

    _eth = reinterpret_cast<ethhdr*>(buffer);
    _iph = reinterpret_cast<iphdr*>(_buffer + sizeof(ethhdr));

    if (getEthernetProtocol() == IP)
    {
        getIp();
    }
}


int Packet::getPacketSize() const
{
    return _size;
}

unsigned int Packet::getIPSize() const
{
    return _iph->ihl * 4;
}

int Packet::getProtocol() const
{
    return _iph->protocol;
}

int Packet::getEthernetProtocol() const
{
    return _eth->h_proto;
}


int Packet::getIPVersion() const
{
    return _iph->version;
}

string Packet::getSourceMac() const
{
    char str[30];
    int n = sprintf(str, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", _eth->h_dest[0], _eth->h_dest[1],
            _eth->h_dest[2], _eth->h_dest[3], _eth->h_dest[4],
            _eth->h_dest[5]);
    str[n] = 0;
    return str;
}


string Packet::getDestMac() const
{
    char str[30];
    int n = sprintf(str, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",_eth->h_source[0], _eth->h_source[1],
            _eth->h_source[2], _eth->h_source[3],
            _eth->h_source[4], _eth->h_source[5]);
    str[n] = 0;
    return str;
}


void Packet::savePacket(Document &doc)
{
    doc.add("destination_adresss", getDestMac());
    doc.add("source_address", getSourceMac());
    doc.add("ethernet_protocol", getEthernetProtocol());
    doc.add("protocol", getProtocol());
    doc.add("ip_version", getIPVersion());
    doc.add("packet_size", getPacketSize());
    doc.add("dest_ip", (Int64)getRawDestIp());
    doc.add("source_ip", (Int64)getRawSourceIp());
}

ostream& Packet::writePacket(ostream &os)
{
    Object obj;
    obj.set("destination_adresss", getDestMac());
    obj.set("source_address", getSourceMac());
    obj.set("ethernet_protocol", getEthernetProtocol());
    obj.set("ip_version", getIPVersion());
    obj.set("packet_size", getPacketSize());
    obj.set("dest_ip", getDestIP());
    obj.set("source_ip", getDestIP());

    obj.stringify(os);

    return os;
}

Packet::~Packet()
{
    delete []_buffer;
}


string Packet::getSourceIP() const
{
    return _sip;
}


string Packet::getDestIP() const
{
    return _dip;
}

UInt32 Packet::getRawDestIp() const
{
    return ntohl(_iph->saddr);
}

UInt32 Packet::getRawSourceIp() const
{
    return ntohl(_iph->daddr);
}

void Packet::getIp()
{
    in_addr source, dest;
    source.s_addr = _iph->saddr;
    dest.s_addr = _iph->daddr;
    _dip = inet_ntoa(dest);
    _sip = inet_ntoa(source);
}










