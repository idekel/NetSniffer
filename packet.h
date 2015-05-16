#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <memory>
#include <iostream>

#include<netinet/ip.h>
#include<net/ethernet.h>

#include <Poco/Notification.h>
#include <Poco/MongoDB/InsertRequest.h>

typedef unsigned char byte;

class Packet : public Poco::Notification
{
public:

    typedef Poco::AutoPtr<Packet> Ptr;

    enum {
        IP = 8
    };

    Packet(byte *buffer, int sz);

    virtual ~Packet();

    virtual int getPacketSize() const;
    virtual unsigned int getIPSize() const;
    virtual int getProtocol() const;
    virtual int getEthernetProtocol() const;
    virtual std::string getDestMac() const;
    virtual std::string getSourceMac() const;
    virtual int getIPVersion() const;
    virtual std::string getSourceIP() const;
    virtual std::string getDestIP() const;

    /**
     * @brief getRawDestIp raw ip in host byte order.
     * @return
     */
    Poco::UInt32 getRawDestIp() const;

    /**
     * @brief getRawSourceIp raw ip in host byte order.
     * @return
     */
    Poco::UInt32 getRawSourceIp() const;

    virtual std::ostream& writePacket(std::ostream &os);
    virtual void savePacket(Poco::MongoDB::Document &doc);

protected:
    unsigned char *_buffer;
    int _size;
    ethhdr *_eth;
    iphdr *_iph;
    std::string _sip, _dip;

private:
    void getIp();
};


#endif // PACKET_H
