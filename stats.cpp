#include "stats.h"
#include "packetfilter.h"

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

    }

}


void Stats::count(TCPPacket *ptr)
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
