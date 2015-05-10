#ifndef NETSNIFFER_H
#define NETSNIFFER_H

#include <Poco/Util/ServerApplication.h>
#include <Poco/NotificationQueue.h>

class NetSniffer : public Poco::Util::ServerApplication
{
public:
    NetSniffer();

    ~NetSniffer();

    void initialize(Poco::Util::Application &self) override;
    int main(const std::vector<std::string> &args) override;

    bool stop();

    Poco::NotificationQueue& getPacketQueue();

    //for cool thread name
    static void setThreadName(int tid, const std::string &name);

private:
    Poco::NotificationQueue _packetQueue;
    bool _stop;
};

#endif // NETSNIFFER_H
