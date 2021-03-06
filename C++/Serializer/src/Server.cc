/*
  c++ -std=c++11 -o Server Server.cc SocketUtils.cc -I $BOOST_ROOT -L $BOOST_ROOT/lib -lboost_serialization
*/

#include <string>
#include <sstream>
#include <iostream>

#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"

#include "DataStructure.h"
#include "SocketUtils.h"

template<typename T>
void DeSerializer(const T& theStream, EventData::DataStructure& theData)
{
  std::istringstream theSerialized(std::string((char*)theStream.data(), theStream.size()));
  boost::archive::binary_iarchive theArchive(theSerialized);
  theArchive >> theData;
}

int main(int argc, char* argv[])
{
  std::string command;

  if (argc < 3)
    {
      std::cout << "Parameter missing: ./Server server_address port" << std::endl;
      exit(EXIT_FAILURE);
    }

  // ###################
  // # Socket creation #
  // ###################
  boost::asio::io_service IOservice;
  boost::asio::ip::tcp::acceptor acceptorServer(IOservice, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(argv[1]), atoi(argv[2])));
  // boost::asio::ip::tcp::acceptor acceptorServer(IOservice, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), atoi(argv[1])));
  boost::asio::ip::tcp::socket serverSocket(IOservice);

  // ##########################
  // # Waiting for connection #
  // ##########################
  std::cout << "Server is listening on address:port = " << argv[1] << ":" << argv[2] << std::endl;
  acceptorServer.accept(serverSocket);

  std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));

  // ################################
  // # Waiting for STARTING command #
  // ################################
  std::cout << "Waiting for STARTING command" << std::endl;
  do
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));
    } while ((command = SocketUtils::getData(serverSocket, SocketUtils::ENDOFMSG)).find("STARTING") == std::string::npos);
  std::cout << "Received command: " << command << std::endl;

  // #########################
  // # Sending START command #
  // #########################
  std::cout << "Sending START command" << std::endl;
  SocketUtils::sendData(serverSocket, "START", SocketUtils::ENDOFMSG);
  std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));

  // ####################
  // # Waiting for DATA #
  // ####################
  EventData::DataStructure theData;
  std::cout << "Waiting for DATA" << std::endl;
  auto theStream = SocketUtils::getData(serverSocket, SocketUtils::ENDOFMSG);
  std::cout << "=== Received DATA ===" << std::endl;
  DeSerializer(theStream, theData);
  std::cout << theData.data1 << std::endl;
  std::cout << theData.data2 << std::endl;
  std::cout << theData.subVec[0].subData1 << std::endl;
  std::cout << theData.subVec[0].subData2 << std::endl;
  std::cout << "=====================" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));

  // ################################
  // # Waiting for STOPPING command #
  // ################################
  std::cout << "Waiting for STOPPING command" << std::endl;
  do
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(SocketUtils::SLEEPTIME));
    } while ((command = SocketUtils::getData(serverSocket, SocketUtils::ENDOFMSG)).find("STOPPING") == std::string::npos);
  std::cout << "Received command: " << command << std::endl;

  // ########################
  // # Sending STOP command #
  // ########################
  std::cout << "Sending STOP command" << std::endl;
  SocketUtils::sendData(serverSocket, "STOP", SocketUtils::ENDOFMSG);

  return 0;
}
