#include "storer.h"
#include "createcollection.h"
#include "netsniffer.h"
#include "packet.h"

#include <fstream>

#include <Poco/File.h>

using namespace std;
using namespace Poco;
using namespace Util;

Storer::Storer() : _config(Application::instance().config())
{
    _pcapFile = _config.getString("pcap_file");
}

Storer::~Storer()
{

}

/*void Storer::createCollection()
{
    auto db = _config.getString("db");
    auto collName = _config.getString("collection");
    auto size = _config.getInt64("size");
    CreateCollection coll(db, collName, true, size);
    ResponseMessage response;
    _con.sendRequest(coll, response);

    if (response.hasDocuments())
    {
        auto docs = response.documents();
        Document::Ptr doc = docs[0];
        cout << doc->toString() << endl;
    }
}*/

bool Storer::init()
{
    File file(_pcapFile);

    if (!file.exists())
    {
        if (!file.createFile())
        {
            return false;
        }
    }

    if (file.getSize() == 0)
    {
        return initFile();
    }

    return true;
}

bool Storer::initFile()
{
    fstream file(_pcapFile, fstream::out);
    if (!file.is_open())
    {
        return false;
    }

    PcapHdr hd;

    BinaryWriter os(file);
    hd.write(os);

    file.flush();

    return true;
}

void Storer::run()
{
    auto &app = static_cast<NetSniffer&>(NetSniffer::instance());
    auto &queue = app.getStorePacketQueue();
    NetSniffer::setThreadName("Storer");
    try
    {

        if (!init())
        {
            throw RuntimeException("could open/create the capture file");
        }

        fstream file(_pcapFile, fstream::app | fstream::binary);

        if (!file.is_open())
        {
            throw RuntimeException("could open/create the capture file");
        }

        BinaryWriter writer(file);

        while(!app.stop())
        {
            auto tmp = queue.waitDequeueNotification();
            if (tmp)
            {
                Packet::Ptr packet = static_cast<Packet*>(tmp);
                packet->writePacket(writer);
                file.flush();
            }
        }
    }
    catch (exception &e)
    {
        app.logger().fatal(e.what());
    }

}

