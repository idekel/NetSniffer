#include "packet.h"

#include <cstring>
#include <cstdio>

#include <Poco/Exception.h>

using namespace std;

Packet::Packet(unsigned char *buffer, int sz) : _size(sz)
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

    _hdr = reinterpret_cast<ethhdr*>(buffer);
    _iph = reinterpret_cast<iphdr*>(_buffer + sizeof(ethhdr));
}


int Packet::getSize()
{
    return _size;
}

int Packet::getProtocol()
{
    return _iph->protocol;
}


int Packet::getIPVersion()
{
    return _iph->version;
}

string Packet::getSourceMac()
{
    char str[30];
    strcpy(str, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X");
    int n = sprintf(str, eth->h_dest[0], eth->h_dest[1],
            eth->h_dest[2], eth->h_dest[3], eth->h_dest[4],
            eth->h_dest[5]);
    str[n] = 0;
    return str;
}


string Packet::getDestMac()
{
    char str[30];
    strcpy(str, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X");
    int n = sprintf(str, eth->h_source[0], eth->h_source[1],
            eth->h_source[2], eth->h_source[3],
            eth->h_source[4], eth->h_source[5]);
    str[n] = 0;
    return str;
}

Packet::~Packet()
{
    delete []_buffer;
}


int Packet::getSize() const
{
    return _size;
}
