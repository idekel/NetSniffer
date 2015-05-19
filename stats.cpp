#include "stats.h"
#include "packetfilter.h"

#include <arpa/inet.h>

using namespace std;
using namespace Poco;

Stats::Stats()
{

}

Stats::~Stats()
{

}



Stats& Stats::count(Packet::Ptr ptr)
{
    UInt32 dip = ptr->getRawDestIp();
    UInt32 sip = ptr->getRawSourceIp();

    Poco::UInt64 key = sip + dip;
    auto &cons = _conversations[key];
    cons.counter(ptr);

    auto prot = ptr->getProtocol();

    auto psize = ptr->getPacketSize();

    _protocols[prot] += psize;

    if (prot == PacketFilter::TCP)
    {
        count(static_cast<TCPPacket*>(ptr.get()));

    } else if (prot == PacketFilter::UDP)
    {
        count(static_cast<UDPPacket*>(ptr.get()));
    }

}


void Stats::count(TCPPacket *ptr)
{
    auto psize = ptr->getPacketSize();
    _ports[ptr->getRawDestIp()] += psize;
    _ports[ptr->getRawSourceIp()] += psize;
}

void Stats::count(UDPPacket *ptr)
{
    auto psize = ptr->getPacketSize();
    _ports[ptr->getRawDestIp()] += psize;
    _ports[ptr->getRawSourceIp()] += psize;
}


void Stats::Conversation::counter(Packet::Ptr ptr)
{
    UInt32 dip = ptr->getRawDestIp();
    UInt32 sip = ptr->getRawSourceIp();

    if (!count)
    {
        source = sip;
        dest = dip;
    }

    count += ptr->getPacketSize();

}


Stats &Stats::print(ostream &os)
{
    os << "\033[2J";
    for (auto &it : _conversations)
    {
        os << Packet::fromRawIP(it.second.source)
           << " >>> "
           << Packet::fromRawIP(it.second.dest)
           << ": " << it.second.count << "\n";
    }
    os.flush();
    return *this;
}
