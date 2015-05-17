#ifndef STATS_H
#define STATS_H

#include <map>

#include <Poco/Types.h>

#include "tcppacket.h"
#include "udppacket.h"

class Stats
{
public:
    Stats();
    ~Stats();


    Stats& count(Packet::Ptr ptr);
    void count(TCPPacket *ptr);
    void count(UDPPacket *ptr);

    struct Conversation
    {
        Conversation()
        {
            count = 0;
            source = 0;
            dest = 0;
        }

        void counter(Packet::Ptr ptr);

        Poco::UInt32 source;
        Poco::UInt32 dest;
        Poco::UInt64 count;
    };

    struct Counter
    {
        Counter() : count(0)
        {

        }

        bool operator == (const Counter& other)
        {
            return count == other.count;
        }

        Counter& operator += (Poco::Int64 sz)
        {
            count += sz;
            return *this;
        }

        Poco::Int64 count;
    };

private:
    std::map<Poco::UInt64, Conversation> _conversations;
    std::map<Poco::Int32, Counter> _ports;
    std::map<Poco::Int8, Counter> _protocols;
    std::map<Poco::UInt32, Counter> _talkers;
    std::map<Poco::UInt32, Counter> _listeners;
};

#endif // STATS_H
