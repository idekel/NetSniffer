#include "netsniffer.h"
#include "packetfilter.h"
#include "packetlistner.h"

#include <fstream>

#include <Poco/ThreadPool.h>

using namespace std;
using namespace Poco;

NetSniffer::NetSniffer() : _stop(false)
{

}

NetSniffer::~NetSniffer()
{

}


void NetSniffer::initialize(Poco::Util::Application &self)
{
    ServerApplication::initialize(self);
}


int NetSniffer::main(const std::vector<std::string> &args)
{

    PacketListner listener;
    PacketFilter filter;

    ThreadPool::defaultPool().start(listener, "Packet Listener");
    ThreadPool::defaultPool().start(filter, "Packet Filter");

    waitForTerminationRequest();

    _stop = true;

    _packetQueue.wakeUpAll();



    return EXIT_OK;
}

bool NetSniffer::stop()
{
    return _stop;
}

NotificationQueue& NetSniffer::getPacketQueue()
{
    return _packetQueue;
}


void NetSniffer::setThreadName(int tid, const string &name)
{
    string file("/proc/self/task/");
    file += to_string(tid) + "/comm";

    fstream of(file, fstream::out);

    if (of.is_open())
    {
        of << name;
    }
}


