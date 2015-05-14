#ifndef STORER_H
#define STORER_H

#include <Poco/Runnable.h>
#include <Poco/MongoDB/Connection.h>
#include <Poco/Util/Application.h>

class Storer : public Poco::Runnable
{
public:
    Storer();
    ~Storer();

    void run() override;

private:
    Poco::MongoDB::Connection _con;
    Poco::Util::LayeredConfiguration &_config;


    void createCollection();
};

#endif // STORER_H
