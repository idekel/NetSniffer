#include "packetlistner.h"
#include "netsniffer.h"
#include "packetfilter.h"
#include "tcppacket.h"
#include "udppacket.h"

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
    //main queue
    auto &queue = app.getPacketQueue();
    //queue for the storer
    auto &storeQueue = app.getStorePacketQueue();
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
                //Notification::Ptr work = new Packet(_buffer, sz);
                auto work = getPacket(_buffer, sz);
                if (work->getDestIP() == "127.0.0.1")
                    continue;
                queue.enqueueNotification(work);
                storeQueue.enqueueNotification(work);
            }
        }
    }

    //when the app is signal for termination this exception
    //is thown
    catch (InvalidArgumentException &ie) {
        //cerr << ie.what() << endl;
    }

    catch (exception &e) {
        app.logger().fatal(e.what());
    }
}


Packet::Ptr PacketListner::getPacket(byte *buf, int sz)
{
    auto iph = reinterpret_cast<iphdr*>(_buffer + sizeof(ethhdr));

    if (iph->protocol == PacketFilter::TCP)
    {
        return new TCPPacket(buf, sz);

    } else if (iph->protocol == PacketFilter::UDP)
    {
        return new UDPPacket(_buffer, sz);
    }

    return new Packet(buf, sz);
}
