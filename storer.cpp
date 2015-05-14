#include "storer.h"
#include "createcollection.h"

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

}

