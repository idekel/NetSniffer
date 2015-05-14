#ifndef CREATECOLLECTION_H
#define CREATECOLLECTION_H

#include <Poco/MongoDB/RequestMessage.h>
#include <Poco/BinaryWriter.h>
#include <Poco/Types.h>


class CreateCollection : public Poco::MongoDB::RequestMessage
{
public:
    CreateCollection(const std::string &db,
                     const std::string &collection,
                     bool capped = false,
                     Poco::Int64 size = 0,
                     Poco::Int32 max = 0,
                     bool autoIndex = false);

    void buildRequest(Poco::BinaryWriter &writer);
    void writeBson(Poco::BinaryWriter& writer);

    ~CreateCollection();

private:
    Poco::Int32 _flags;
    std::string _fullCollectionName, _collection;
    Poco::Int32       _numberToSkip;
    Poco::Int32       _numberToReturn;
    bool _capped, _autoIndex;
    Poco::Int32 _max;
    Poco::Int64 _size;
};

#endif // CREATECOLLECTION_H
