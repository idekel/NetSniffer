#ifndef PACKETFILTER_H
#define PACKETFILTER_H

#include <memory>
#include <mutex>

#include <Poco/NotificationQueue.h>
#include <Poco/Runnable.h>

#include "packet.h"

class PacketFilter : public Poco::Runnable
{
public:

    enum {
        ICMP = 1,
        IGMP = 2,
        TCP = 6,
        UDP = 17
    };


    PacketFilter();
    ~PacketFilter();

    void print();

    void run() override;

private:
    static long _tcp, _udp, _icmp, _igmp, _other;
};

#endif // PACKETFILTER_H
