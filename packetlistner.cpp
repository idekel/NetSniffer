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
    NetSniffer::setThreadName("Packet Listener");
    try {
        auto de = app.config().getString("interface", "wlan0");
        _socket.impl()->setRawOption(SOL_SOCKET,
                                     SO_BINDTODEVICE,
                                     de.c_str(),
                                     de.length()+1);
        while (!app.stop())
        {
            int sz = _socket.impl()->receiveBytes(_buffer, BUFSIZE);
            if (sz){
                Notification::Ptr work = new Packet(_buffer, sz);
                queue.enqueueNotification(work);
            }
        }
    }

    //when the app is signal for termination this exceptio
    //is thown
    catch (InvalidArgumentException &ie) {
        //cerr << ie.what() << endl;
    }

    catch (exception &e) {
        app.logger().fatal(e.what());
    }
}

