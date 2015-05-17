#include "filter.h"

using namespace std;
using namespace Poco;
using namespace JSON;


Filter::Filter(const string name) : _filtername(name)
{
    _hasfilter = false;
}

Filter::Filter(const string &name, Object::Ptr conf) : _filtername(name)
{
    _hasfilter = true;

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
        auto range = conf->get("range").toString();

    }
}

Filter::~Filter()
{

}

