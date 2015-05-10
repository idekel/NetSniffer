#include "packetlistner.h"
#include "netsniffer.h"


using namespace std;
using namespace Poco;

int PacketListner::BUFSIZE = 65536;

PacketListner::PacketListner() : _buffer(new byte[BUFSIZE])
{

}

PacketListner::~PacketListner()
{
    delete []_buffer;
}


void PacketListner::run()
{
    auto &app = static_cast<NetSniffer&>(NetSniffer::instance());
    auto &queue = app.getPacketQueue();
    NetSniffer::setThreadName(Thread::currentTid(), "Packet Listener");
    while (!app.stop())
    {
        int sz = _socket.impl()->receiveBytes(_buffer, BUFSIZE);
        Notification::Ptr work = new Packet(_buffer, sz);
        queue.enqueueNotification(work);
    }
}

