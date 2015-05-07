#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <memory>

#include<netinet/ip.h>
#include<net/ethernet.h>

class Packet
{
public:

    typedef std::shared_ptr<Packet> Ptr;

    Packet(unsigned char *buffer, int sz);

    virtual ~Packet();

    virtual int getSize() const;
    virtual int getProtocol() const;
    virtual int getIPProtocol() const;
    virtual std::string getDestMac() const;
    virtual std::string getSourceMac() const;
    virtual int getIPVersion() const;
    virtual std::string getSourceIP() const;
    virtual std::string getDestIP() const;


protected:
    unsigned char *_buffer;
    int _size;
    ethhdr *_hdr;
    iphdr *_iph;
};


#endif // PACKET_H
