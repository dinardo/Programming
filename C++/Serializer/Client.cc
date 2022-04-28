/*
  c++ -std=c++11 -o Client Client.cc SocketUtils.cc -I $BOOST_ROOT -L $BOOST_ROOT/lib -lboost_serialization
*/

#include <string>
#include <sstream>
#include <iostream>

#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"

#include "DataStructure.h"
#include "SocketUtils.h"

void Serializer(const EventData::DataStructure& theData, std::string& theStream)
{
  std::ostringstream theSerialized;
  boost::archive::binary_oarchive theArchive(theSerialized);
  theArchive << theData;
  theStream = theSerialized.str();
}

int main(int argc, char* argv[])
{
  std::string command;

  if (argc < 3)
    {
      std::cout << "Parameter missing: ./Client server_address port" << std::endl;
      exit(EXIT_FAILURE);
    }

  // ###################
  // # Socket creation #
  // ###################
  boost::asio::io_service IOservice;
  boost::asio::ip::tcp::socket clientSocket(IOservice);
  clientSocket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(argv[1]), atoi(argv[2])));

  std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));

  // ####################
  // # Serializing data #
  // ####################
  EventData::DataStructure theData{1, 2.3, {}};
  theData.subVec.push_back({2, 3.3});

  std::string theStream;
  Serializer(theData, theStream);

  // ############################
  // # Sending STARTING command #
  // ############################
  std::cout << "Sending STARTING command" << std::endl;
  do
    {
      SocketUtils::sendData(clientSocket, "STARTING", SocketUtils::ENDOFMSG);
      std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));
    } while ((command = SocketUtils::getData(clientSocket, SocketUtils::ENDOFMSG)).find("START") == std::string::npos);
  std::cout << "Received command: " << command << std::endl;

  // ################
  // # Sending DATA #
  // ################
  std::cout << "Sending DATA" << std::endl;
  SocketUtils::sendData(clientSocket, theStream, SocketUtils::ENDOFMSG);
  std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));

  // ############################
  // # Sending STOPPING command #
  // ############################
  std::cout << "Sending STOPPING command" << std::endl;
  do
    {
      SocketUtils::sendData(clientSocket, "STOPPING", SocketUtils::ENDOFMSG);
      std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));
    } while ((command = SocketUtils::getData(clientSocket, SocketUtils::ENDOFMSG)).find("STOP") == std::string::npos);
  std::cout << "Received command: " << command << std::endl;

  return 0;
}
