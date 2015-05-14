#include "createcollection.h"

#include <Poco/MongoDB/BSONWriter.h>
#include <Poco/MongoDB/Element.h>

using namespace std;
using namespace Poco;
using namespace MongoDB;

CreateCollection::CreateCollection(const string &db,
                                   const string &collection,
                                   bool capped,
                                   Int64 size,
                                   Int32 max, bool autoIndex)
    : RequestMessage(MessageHeader::Query),
      _fullCollectionName(db + ".$cmd"),
      _collection(collection),
      _capped(capped),
      _size(size),
      _max(max),
      _flags(0),
      _numberToReturn(1),
      _numberToSkip(0),
      _autoIndex(autoIndex)
{

}

CreateCollection::~CreateCollection()
{

}


void CreateCollection::buildRequest(BinaryWriter &writer)
{
    writer << _flags;
    BSONWriter(writer).writeCString(_fullCollectionName);
    writer << _numberToSkip;
    writer << _numberToReturn;
    writeBson(writer);
}


void CreateCollection::writeBson(BinaryWriter &writer)
{
    stringstream ss;
    BinaryWriter tmpWriter(ss);
    //create
    tmpWriter << (unsigned char) ElementTraits<string>::TypeId;
    BSONWriter bwriter(tmpWriter);
    bwriter.writeCString("create");
    bwriter.write(_collection);
    //capped
    if (_capped)
    {
        tmpWriter << (unsigned char) ElementTraits<bool>::TypeId;
        bwriter.writeCString("capped");
        bwriter.write<bool>(_capped);
        //size
        tmpWriter << (unsigned char) ElementTraits<Poco::Int64>::TypeId;
        bwriter.writeCString("size");
        if (!_size)
        {
            throw Poco::RuntimeException("invalid max size: " + to_string(_size));
        }
        bwriter.write(_size);
    }

    //max
    if (_max)
    {
        tmpWriter << (unsigned char) ElementTraits<Poco::Int32>::TypeId;
        bwriter.writeCString("max");
        bwriter.write(_max);
    }

    //autoIndex
    if (_autoIndex)
    {
        tmpWriter << (unsigned char) ElementTraits<bool>::TypeId;
        bwriter.writeCString("autoIndexID");
        bwriter.write<bool>(_autoIndex);
    }

    tmpWriter.flush();

    Poco::Int32 len = static_cast<Poco::Int32>(5 + ss.tellp()); /* 5 = sizeof(len) + 0-byte */
    writer << len;
    writer.writeRaw(ss.str());

    writer << '\0';
}







