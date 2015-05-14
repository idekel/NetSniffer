#include "packetfilter.h"
#include "netsniffer.h"

#include <stdexcept>

using namespace std;
using namespace Poco;


long PacketFilter::_icmp = 0;
long PacketFilter::_igmp = 0;
long PacketFilter::_tcp = 0;
long PacketFilter::_udp = 0;
long PacketFilter::_other = 0;



PacketFilter::PacketFilter()
{

}


PacketFilter::~PacketFilter()
{

}

void PacketFilter::run()
{
    auto &app = static_cast<NetSniffer&>(NetSniffer::instance());
    NetSniffer::setThreadName("Packet Filter");
    try {

        auto &queue = app.getPacketQueue();

        while(!app.stop())
        {
            auto tmp = queue.waitDequeueNotification();
            if (tmp){
                Packet::Ptr ptr(static_cast<Packet*>(tmp));

                switch (ptr->getProtocol()) //Check the Protocol and do accordingly...
                {
                case ICMP:  //ICMP Protocol
                    ++_icmp;
                    break;

                case IGMP:  //IGMP Protocol
                    ++_igmp;
                    break;

                case TCP:  //TCP Protocol
                    ++_tcp;
                    break;

                case UDP: //UDP Protocol
                    ++_udp;
                    break;

                default: //Some Other Protocol like ARP etc.
                    ++_other;
                    break;
                }
            }

            //for testing porpos
            print();
        }

    }
    catch (exception &e) {
        cerr << e.what() << endl;
    }
}



void PacketFilter::print()
{
    string format = "TCP : %d   UDP : %d   ICMP : %d    "
                    "IGMP : %d   Others : %d\r";
    printf(format.c_str(), _tcp, _udp, _icmp, _igmp, _other);
}






