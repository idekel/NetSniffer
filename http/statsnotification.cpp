#include "statsnotification.h"


using namespace Poco::JSON;

StatsNotification::StatsNotification(Object::Ptr ptr)
{
    _data = ptr;
}

StatsNotification::~StatsNotification()
{

}

Object::Ptr StatsNotification::getData() const
{
    return _data;
}
