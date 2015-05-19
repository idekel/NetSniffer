#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <memory>
#include <iostream>

#include <netinet/ip.h>
#include <net/ethernet.h>

#include <Poco/Notification.h>
#include <Poco/BinaryWriter.h>
#include <Poco/Timestamp.h>


typedef unsigned char byte;


//taken form wireshark documantaction no
//need to change a good example. :)

struct PcapHdr {

        PcapHdr()
        {
            magic_number = 0xa1b2c3d4;
            version_major = 2;
            version_minor = 4;
            thiszone = 0;
            sigfigs = 0;
            snaplen = 65536;
            network = 1;
        }

        Poco::UInt32 magic_number;   /* magic number */
        Poco::UInt16 version_major;  /* major version number */
        Poco::UInt16 version_minor;  /* minor version number */
        Poco::UInt32  thiszone;       /* GMT to local correction */
        Poco::UInt32 sigfigs;        /* accuracy of timestamps */
        Poco::UInt32 snaplen;        /* max length of captured packets, in octets */
        Poco::UInt32 network;        /* data link type */

        Poco::BinaryWriter& write(Poco::BinaryWriter& os)
        {
            os << magic_number;
            os << version_major;
            os << version_minor;
            os << thiszone;
            os << sigfigs;
            os << snaplen;
            os << network;

            return os;
        }
};

struct PCapHd {
        Poco::UInt32 ts_sec;         /* timestamp seconds */
        Poco::UInt32 ts_usec;        /* timestamp microseconds */
        Poco::UInt32 incl_len;       /* number of octets of packet saved in file */
        Poco::UInt32 orig_len;       /* actual length of packet */

        Poco::BinaryWriter& write(Poco::BinaryWriter& os)
        {
            os << ts_sec;
            os << ts_usec;
            os << incl_len;
            os << orig_len;

            return os;
        }
};


class Packet : public Poco::Notification
{
public:

    typedef Poco::AutoPtr<Packet> Ptr;

    enum {
        IP = 8
    };

    Packet(byte *buffer, int sz);
    Packet(byte *buffer, int sz, Poco::Timestamp::TimeVal t);

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

    virtual Poco::BinaryWriter& writePacket(Poco::BinaryWriter &os);


    /**
     * @brief getTime capture packet time
     * @return
     */
    const Poco::Timestamp& getTime();

    /**
     * @brief toRawIP raw ip in host byte order.
     * @return
     */
    static Poco::UInt32 toRawIP(const std::string &ip);
    static std::string fromRawIP(Poco::UInt32 ip);

    //Packet::Ptr readPacket();

protected:
    unsigned char *_buffer;
    int _size;
    ethhdr *_eth;
    iphdr *_iph;
    std::string _sip, _dip;
    Poco::Timestamp _time;

    void init(byte *buffer);

private:
    void getIp();
};


#endif // PACKET_H
