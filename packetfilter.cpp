#include "packetfilter.h"
#include "netsniffer.h"

#include <stdexcept>
#include <fstream>

#include <arpa/inet.h>

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/File.h>


using namespace std;
using namespace Poco;
using namespace JSON;
using Dynamic::Var;


long PacketFilter::_icmp = 0;
long PacketFilter::_igmp = 0;
long PacketFilter::_tcp = 0;
long PacketFilter::_udp = 0;
long PacketFilter::_other = 0;



PacketFilter::PacketFilter()
{
    getFilters();
}


PacketFilter::~PacketFilter()
{

}


void PacketFilter::getFilters()
{
    auto &config = Util::Application::instance().config();
    auto filename = config.getString("filters", "");
    if (!filename.empty())
    {
        File file(filename);
        if (!file.exists())
        {
            throw FileNotFoundException("fileter config file not found");
        }

        fstream os(filename);

        Parser parser;
        Var result = parser.parse(os);
        auto tmp = result.extract<Object::Ptr>();
        Object &ret = *tmp;

        for (pair<const std::string, Var> &it : ret)
        {
            Filter filter(it.first, it.second.extract<Object::Ptr>());
            Stats st;
            _filters[filter] = st;
        }

    }

    auto gen = config.getBool("generalStats", true);

    if (gen)
    {
        Filter ft("general");
        Stats st;
        _filters[ft] = st;
    }

    if (!_filters.size())
    {
        throw RuntimeException("you should a lest allow a genral filter for colletin stats");
    }
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

                for (auto &it : _filters)
                {
                    if (it.first.filter(ptr))
                    {
                        it.second.count(ptr);
                        it.second.print(cout);
                        break;
                    }
                }
            }
        }

    }
    catch (exception &e) {
        cerr << e.what() << endl;
    }
}


void PacketFilter::packetCounter(Packet::Ptr ptr)
{
    auto dip = ptr->getRawDestIp();
    auto sip = ptr->getRawSourceIp();

    _packetIP[dip] += 1;
    _packetIP[sip] += 1;

}


void PacketFilter::print()
{
    //string format = "TCP : %d   UDP : %d   ICMP : %d    "
    //                "IGMP : %d   Others : %d\r";
    //printf(format.c_str(), _tcp, _udp, _icmp, _igmp, _other);

    printf("\033[2J"); // Clear screen

    for (auto &it : _packetIP)
    {
        in_addr ip;
        ip.s_addr = htonl(it.first);
        cout << inet_ntoa(ip) << ": " << it.second.count << endl;
    }
}






