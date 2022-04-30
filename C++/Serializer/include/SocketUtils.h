#include <string>
#include <iostream>

#include <boost/asio.hpp>

namespace SocketUtils
{
  const std::string ENDOFMSG = "ENDOFMSG";
  const int SLEEPTIME = 5; // [ms]

  std::string getData(boost::asio::ip::tcp::socket& socket, const std::string& endOfMsg);
  void sendData(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& endOfMsg);
}
