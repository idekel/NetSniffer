#ifndef PACKETFILTER_H
#define PACKETFILTER_H

#include <memory>
#include <mutex>
#include <map>

#include <Poco/NotificationQueue.h>
#include <Poco/Runnable.h>

#include "packet.h"
#include "filter.h"


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
    void getFilters();

    void run() override;
    void packetCounter(Packet::Ptr ptr);

private:
    static long _tcp, _udp, _icmp, _igmp, _other;
    std::map<Poco::Int64, Stats::Counter> _packetIP;

    std::vector<Filter> _filters;

};

#endif // PACKETFILTER_H
