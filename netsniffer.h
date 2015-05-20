#ifndef NETSNIFFER_H
#define NETSNIFFER_H

#include <memory>

#include <Poco/Util/ServerApplication.h>
#include <Poco/NotificationQueue.h>

#include "packetlistner.h"

class NetSniffer : public Poco::Util::ServerApplication
{
public:
    NetSniffer();

    ~NetSniffer();

    void initialize(Poco::Util::Application &self) override;
    int main(const std::vector<std::string> &args) override;

    bool stop();

    Poco::NotificationQueue& getPacketQueue();
    Poco::NotificationQueue& getStorePacketQueue();

    //for a cool thread name on linux
    static void setThreadName(const std::string &name);

    //start to catch packets
    void startListener();

private:
    Poco::NotificationQueue _packetQueue, _storePacket;
    std::shared_ptr<PacketListner> _listener;
    bool _stop;
};

#endif // NETSNIFFER_H
