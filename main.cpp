#include <iostream>
#include <vector>

#include<netinet/ip.h>
#include<net/ethernet.h>

#include "rawsocket.h"
#include "packet.h"



using namespace std;

void processPacket(Packet::Ptr ptr);

int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0, bufLen=0, mi=0, ma=0;
double avg=0.0;

int main()
{

    auto buffer = new unsigned char[65536];
    RawSocket sock;

    int sz = 0;
    do {

        sz = sock.impl()->receiveBytes(buffer, 65536);
        Packet::Ptr ptr(new Packet(buffer, sz));
        processPacket(ptr);

    }while (sz);


    return 0;
}

void processPacket(Packet::Ptr ptr)
{
    //Get the IP Header part of this packet , excluding the ethernet header
    //struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ++total;
    int size = ptr->getPacketSize();
    bufLen += size;
    if (!avg)
    {
        avg = size / total;
        mi = size;
        ma = size;
    }
    else
    {
        avg = bufLen / total;
        if (size > ma){
            ma = size;
        }

        if (size < mi){
            mi = size;
        }
    }
    switch (ptr->getProtocol()) //Check the Protocol and do accordingly...
    {
    case 1:  //ICMP Protocol
        ++icmp;
        //print_icmp_packet( buffer , size);
        break;

    case 2:  //IGMP Protocol
        ++igmp;
        break;

    case 6:  //TCP Protocol
        ++tcp;
        //print_tcp_packet(buffer , size);
        break;

    case 17: //UDP Protocol
        ++udp;
        //print_udp_packet(buffer , size);
        break;

    default: //Some Other Protocol like ARP etc.
        ++others;
        break;
    }
    string format = "TCP : %d   UDP : %d   ICMP : %d"
            "IGMP : %d   Others : %d   Total : %d   "
            "BufAvg: %d Min: %d  "
            "Max:  %d\r";
    printf(format.c_str(), tcp , udp , icmp , igmp , others , total, avg, size, mi, ma);
}
