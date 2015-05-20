#include "netsniffer.h"
#include "packetfilter.h"
#include "storer.h"

#include <signal.h>

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

static void restartListener(int sig)
{
    if (sig == SIGUSR1)
    {
        auto &app = static_cast<NetSniffer&>(NetSniffer::instance());
        app.startListener();
    }
}


void NetSniffer::initialize(Poco::Util::Application &self)
{
    ServerApplication::initialize(self);
    loadConfiguration();
}


void NetSniffer::startListener()
{
    if (!_listener)
    {
        _listener.reset(new PacketListner);

    } else if (!_listener->isRunning())
    {
        _listener.reset(new PacketListner);

    } else
    {
        return;
    }

    //logger().information("restaring listener");

    ThreadPool::defaultPool().start(*_listener, "Packet Listener");
}

int NetSniffer::main(const std::vector<std::string> &args)
{

    try {

        Storer store;
        PacketFilter filter;
        ThreadPool::defaultPool().start(filter, "Packet Filter");
        ThreadPool::defaultPool().start(store, "Storer");

        startListener();

        //if some error cause the listener to stop
        //it can be restarted with SIGUSR1
        struct sigaction new_action;
        new_action.sa_handler = restartListener;
        sigemptyset (&new_action.sa_mask);
        new_action.sa_flags = 0;
        sigaction (SIGUSR1, &new_action, NULL);

        waitForTerminationRequest();

        _stop = true;

        _packetQueue.wakeUpAll();

        ThreadPool::defaultPool().joinAll();

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


