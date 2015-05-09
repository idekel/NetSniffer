#include "packetfilter.h"

#include <stdexcept>

using namespace std;

PacketFilter::PacketFilter()
{
    if (_instance)
    {
        throw runtime_error("this is a sigleton class");
    }

    _instance = this;
}


PacketFilter::~PacketFilter()
{

}

long PacketFilter::_icmp = 0;
long PacketFilter::_igmp = 0;
long PacketFilter::_tcp = 0;
long PacketFilter::_udp = 0;
long PacketFilter::_other = 0;
