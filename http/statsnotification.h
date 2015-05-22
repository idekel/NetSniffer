#ifndef STATSNOTIFICATION_H
#define STATSNOTIFICATION_H

#include <Poco/Notification.h>
#include <Poco/JSON/Object.h>

class StatsNotification : public Poco::Notification
{
public:
    StatsNotification(Poco::JSON::Object::Ptr ptr);

    ~StatsNotification();

    Poco::JSON::Object::Ptr getData() const;

private:
    Poco::JSON::Object::Ptr _data;
};

#endif // STATSNOTIFICATION_H
