#include "storer.h"
#include "createcollection.h"
#include "netsniffer.h"
#include "packet.h"

#include <Poco/MongoDB/Database.h>

using namespace std;
using namespace Poco::Util;
using namespace Poco::MongoDB;

Storer::Storer() : _config(Application::instance().config())
{
    auto host = _config.getString("host");
    auto port = _config.getInt("port");
    _con.connect(host, port);
    createCollection();
}

Storer::~Storer()
{

}

void Storer::createCollection()
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
}



void Storer::run()
{
    auto &app = static_cast<NetSniffer&>(NetSniffer::instance());
    auto &queue = app.getStorePacketQueue();
    NetSniffer::setThreadName("Storer");
    try
    {
        Database db(_config.getString("db"));
        auto collName = _config.getString("collection");
        while(!app.stop())
        {
            auto tmp = queue.waitDequeueNotification();
            if (tmp)
            {
                Packet::Ptr packet = static_cast<Packet*>(tmp);
                auto request = db.createInsertRequest(collName);
                packet->savePacket(request->addNewDocument());
                _con.sendRequest(*request);
            }
        }
    }
    catch (exception &e)
    {

    }
}

