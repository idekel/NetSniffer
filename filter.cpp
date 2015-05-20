#include "filter.h"
#include "packetfilter.h"

#include <algorithm>

using namespace std;
using namespace Poco;
using namespace JSON;
using Dynamic::Var;


Filter::Filter(const string name) : _hasfilter(false),
    _filtername(name)
{

}

Filter::Filter(const string &name, Object::Ptr conf) : _hasfilter(true),
    _filtername(name),
    _range(0),
    _netmask(0),
    _minPort(0),
    _maxPort(0)
{

    if (conf->has("minPort"))
    {
        _minPort = conf->get("minPort").extract<UInt16>();
    }

    if (conf->has("maxPort"))
    {
        _maxPort = conf->get("minPort").extract<UInt16>();
    }

    if (conf->has("range"))
    {
        auto str = conf->get("range").toString();
        auto pos = str.find('/');
        auto range = str.substr(0, pos);
        auto mask = str.substr(pos);
        _netmask = getMask(stoul(mask));
        _range = Packet::toRawIP(range);
    }

    if (conf->has("protocols"))
    {
        auto pro = conf->get("protocols");
        if (!pro.isArray())
        {
            throw RuntimeException("protocols should be an array");
        }

        auto arr = pro.extract<Array>();

        for (auto &it : arr)
        {
            _protocols.push_back(it.extract<UInt8>());
        }
    }

    if (conf->has("ports"))
    {
        auto por = conf->get("ports");
        if (!por.isArray())
        {
            throw RuntimeException("ports should be an array");
        }

        auto arr = por.extract<Array>();

        for (auto &it : arr)
        {
            _ports.push_back(it.extract<UInt16>());
        }
    }
}

Filter::~Filter()
{

}


bool Filter::filter(Packet::Ptr ptr) const
{

    if (_hasfilter)
    {
        if (_range && _netmask)
        {
            if (!isInRange(ptr->getRawDestIp())
                    && !isInRange(ptr->getRawSourceIp()))
                return false;
        }

        auto protocol = ptr->getProtocol();
        if (_protocols.size())
        {
            if (find(_protocols.begin(), _protocols.end(), protocol)
                    == _protocols.end())
                return false;
        }
        if (_ports.size())
        {
            if (protocol == PacketFilter::TCP)
            {
                auto tcp = static_cast<TCPPacket*>(ptr.get());
                if (find(_ports.begin(),
                         _ports.end(),
                         tcp->getSourcePort())
                        == _ports.end())
                {
                    return false;
                }

                if (find(_ports.begin(),
                         _ports.end(),
                         tcp->getDestPort())
                        == _ports.end())
                {
                    return false;
                }
            }

            if (protocol == PacketFilter::UDP)
            {
                auto udp = static_cast<UDPPacket*>(ptr.get());
                if (find(_ports.begin(),
                         _ports.end(),
                         udp->getSourcePort())
                        == _ports.end())
                {
                    return false;
                }

                if (find(_ports.begin(),
                         _ports.end(),
                         udp->getDestPort())
                        == _ports.end())
                {
                    return false;
                }
            }
        }


        return true;
    }

    return true;
}





