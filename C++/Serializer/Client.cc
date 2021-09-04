/*
  c++ -o Client Client.cc -I $BOOST
*/

#include <iostream>

#include <boost/asio.hpp>

std::string getData(boost::asio::ip::tcp::socket& socket)
{
  boost::asio::streambuf buf;
  boost::asio::read_until(socket, buf, "\n");
  std::string data = boost::asio::buffer_cast<const char*>(buf.data());

  return data;
}

void sendData(boost::asio::ip::tcp::socket& socket, const std::string& message)
{
  boost::asio::write(socket, boost::asio::buffer(message + "\n"));
}

int main(int argc, char* argv[])
{
  boost::asio::io_service io_service;
  // socket creation
  boost::asio::ip::tcp::socket client_socket(io_service);

  client_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999));

  // Getting username from user
  std::cout << "Enter your name: ";
  std::string u_name, reply, response;
  getline(std::cin, u_name);

  // Sending username to another end
  // to initiate the conversation
  sendData(client_socket, u_name);

  // Infinite loop for chit-chat
  while (true)
    {
    // Fetching response
    response = getData(client_socket);

    // Popping last character "\n"
    response.pop_back();

    // Validating if the connection has to be closed
    if (response == "exit")
      {
        std::cout << "Connection terminated" << std::endl;
        break;
      }
    std::cout << "Server: " << response << std::endl;

    // Reading new message from input stream
    std::cout << u_name << ": ";
    getline(std::cin, reply);
    sendData(client_socket, reply);

    if (reply == "exit") break;
  }

  return 0;
}
