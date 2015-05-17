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

    Poco::UInt32 getMask(Poco::UInt32 bits)
    {
        return ~(~UInt32(0) >> bits);
    }

private:
    Stats _stats;
    bool _hasfilter;
    Poco::UInt32 _netmask;
    Poco::UInt32 _range;
    std::string _filtername;
    Poco::UInt16 _minPort;
    Poco::UInt16 _maxPort;

    std::vector<Poco::UInt8> _protocols;
};

#endif // FILTER_H
