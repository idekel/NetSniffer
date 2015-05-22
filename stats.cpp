#include "stats.h"
#include "packetfilter.h"

#include <arpa/inet.h>

#include <Poco/JSON/Array.h>

using namespace std;
using namespace Poco;
using namespace JSON;

Stats::Stats() : _allData(0)
{

}

Stats::~Stats()
{

}



Stats& Stats::count(Packet::Ptr ptr)
{
    setTransfRate(ptr);

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

    _listeners[dip] += psize;
    _talkers[sip] += psize;

}


void Stats::setTransfRate(Packet::Ptr ptr)
{
    auto &t = ptr->getTime();
    if (_time.epochTime() != t.epochTime())
    {
        _time = t;
        _rate = _allData;
        _allData = ptr->getPacketSize();
    }else
    {
       _allData += ptr->getPacketSize();
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

Object::Ptr Stats::toJson()
{
    Object::Ptr obj(new Object);
    Array convs, talkers, listener, ports, protocols;
    for (auto &con : _conversations)
    {
        convs.add(con.second.toJson());
    }

    obj->set("conversations", convs);

    for (auto &tk : _talkers)
    {
        Object tmp;
        tmp.set("ip", Packet::fromRawIP(tk.first));
        tmp.set("count", tk.second.count);
        talkers.add(tmp);
    }

    obj->set("talkers", talkers);

    for (auto &pts : _ports)
    {
        Object tmp;
        tmp.set("port", pts.first);
        tmp.set("count", pts.second);
        ports.add(tmp);
    }

    obj->set("ports", ports);

    for (auto &lrs : _listeners)
    {
        Object tmp;
        tmp.set("ip", Packet::fromRawIP(lrs.first));
        tmp.set("count", lrs.second.count);
        listener.add(tmp);
    }

    obj->set("listeners", listener);

    for (auto &pls : _protocols)
    {
        Object tmp;
        tmp.set("protocol", pls.first);
        tmp.set("count", pls.second);
        protocols.add(tmp);
    }

    obj->set("protocols", protocols);

    return obj;
}


Object Stats::Conversation::toJson()
{
    Object obj;
    obj.set("source", Packet::fromRawIP(source));
    obj.set("dest", Packet::fromRawIP(dest));
    obj.set("count", count);
    return obj;
}


Stats &Stats::print(ostream &os)
{
    os << "\033[2J";
    if (_rate)
        os << "rate: " << _rate / 1024 << " kbps\n";
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
