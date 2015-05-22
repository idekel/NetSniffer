#include "wsession.h"

using namespace std;
using namespace Poco;
using namespace Net;

WSession::WSession(HTTPServerRequest &request, HTTPServerResponse &response) : WebSocket(request, response)
{

}

WSession::~WSession()
{

}

