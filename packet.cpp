#include "packet.h"

#include <arpa/inet.h>

#include <cstring>
#include <cstdio>

#include <Poco/Exception.h>
#include <Poco/JSON/Object.h>

using namespace std;
using namespace Poco;
using namespace JSON;

Packet::Packet(byte *buffer, int sz) : _size(sz)
{
    init(buffer);
}


Packet::Packet(byte *buffer, int sz, Timestamp::TimeVal t)
    : _size(sz),
      _time(t)
{
    init(buffer);
}

const Timestamp& Packet::getTime()
{
    return _time;
}

Poco::UInt32 Packet::toRawIP(const string &ip)
{
    in_addr add;

    memset(&add, 0, sizeof(in_addr));

    inet_aton(ip.c_str(), &add);

    return ntohl(add.s_addr);
}

string Packet::fromRawIP(UInt32 ip)
{
    in_addr add;
    add.s_addr = htonl(ip);
    return inet_ntoa(add);
}

void Packet::init(byte *buffer)
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


BinaryWriter &Packet::writePacket(BinaryWriter &os, int incl_len)
{
    PCapHd phd;

    phd.ts_sec = _time.epochTime();
    UInt32 usec = _time.epochMicroseconds() - (phd.ts_sec * 1000000);
    phd.ts_usec = usec;
    phd.incl_len = incl_len;
    phd.orig_len = getPacketSize();

    phd.write(os);

    os.writeRaw((char*)_buffer, incl_len);

    os.flush();

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










