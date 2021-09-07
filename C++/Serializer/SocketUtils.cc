#include "SocketUtils.h"

std::string SocketUtils::getData(boost::asio::ip::tcp::socket& socket, const std::string& endOfMsg)
{
  boost::asio::streambuf buf;
  boost::asio::read_until(socket, buf, endOfMsg);
  std::string data = std::string(boost::asio::buffer_cast<const char*>(buf.data()), buf.size() - endOfMsg.size());

  return data;
}

void SocketUtils::sendData(boost::asio::ip::tcp::socket& socket, const std::string& message, const std::string& endOfMsg)
{
  boost::asio::write(socket, boost::asio::buffer(message + endOfMsg));
}
