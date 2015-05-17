#ifndef FILTER_H
#define FILTER_H

#include <vector>

#include <Poco/JSON/Object.h>

#include "stats.h"

class Filter
{
public:
    Filter();
    Filter(Poco::JSON::Object &conf);

    ~Filter();

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
