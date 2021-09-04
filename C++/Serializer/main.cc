/*
  c++ -std=c++11 -o main main.cc -I $BOOST -L $BOOST/lib -lboost_serialization
*/

#include <string>
#include <sstream>
#include <iostream>

#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"

#include "DataStructure.h"

void Serializer(const EventData::DataStructure theData, std::string& theStream)
{
  std::ostringstream theSerialized;
  boost::archive::binary_oarchive theArchive(theSerialized);
  theArchive << theData;
  theStream = theSerialized.str();
}

void DeSerializer(const std::string& theStream, EventData::DataStructure& theData)
{
  std::istringstream theSerialized(std::string((char*)theStream.data(), theStream.size()));
  boost::archive::binary_iarchive theArchive(theSerialized);
  theArchive >> theData;
}

int main()
{
  EventData::DataStructure theDataOut;

  EventData::DataStructure theDataIn{1, 2.3, {}};
  theDataIn.subVec.push_back({2, 3.3});

  std::string theStream;

  Serializer(theDataIn, theStream);
  DeSerializer(theStream, theDataOut);

  std::cout << theDataOut.data1 << std::endl;
  std::cout << theDataOut.data2 << std::endl;
  std::cout << theDataOut.subVec[0].subData1 << std::endl;
  std::cout << theDataOut.subVec[0].subData2 << std::endl;

  return 0;
}
