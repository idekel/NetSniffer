#include "netsniffer.h"
#include "packetfilter.h"
#include "packetlistner.h"
#include "storer.h"

#include <fstream>

#include <sys/prctl.h>

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
    loadConfiguration();
}


int NetSniffer::main(const std::vector<std::string> &args)
{

    try {

        Storer store;

        PacketListner listener;
        PacketFilter filter;

        ThreadPool::defaultPool().start(listener, "Packet Listener");
        ThreadPool::defaultPool().start(filter, "Packet Filter");
        ThreadPool::defaultPool().start(store, "Storer");

        waitForTerminationRequest();

        _stop = true;

        _packetQueue.wakeUpAll();

    }catch (exception &e){
        cerr << e.what() << endl;
    }

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

NotificationQueue& NetSniffer::getStorePacketQueue()
{
    return  _storePacket;
}

void NetSniffer::setThreadName(const string &name)
{    
    if (prctl(PR_SET_NAME, name.c_str()))
    {
        cerr << "error: " << strerror(errno) << endl;
        exit(1);
    }
}


