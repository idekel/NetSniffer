#ifndef FILTER_H
#define FILTER_H

#include <vector>

#include <Poco/JSON/Object.h>

#include "stats.h"

class Filter
{
public:
    Filter(const std::string name);
    Filter(const std::string &name, Poco::JSON::Object::Ptr conf);

    ~Filter();

    bool isInRange(Poco::UInt32 ip) const
    {
        return (ip & _netmask) == (_range & _netmask);
    }

    Poco::UInt32 getMask(Poco::UInt32 bits) const
    {
        return ~(~Poco::UInt32(0) >> bits);
    }

    bool filter(Packet::Ptr ptr) const;

    bool operator <(const Filter &one) const
    {
        return _filtername < one._filtername;
    }

private:
    Stats _stats;
    bool _hasfilter;
    std::string _filtername;
    Poco::UInt32 _netmask;
    Poco::UInt32 _range;
    Poco::UInt16 _minPort;
    Poco::UInt16 _maxPort;

    std::vector<Poco::UInt8> _protocols;
    std::vector<Poco::UInt16> _ports;
};

#endif // FILTER_H
