#include <boost/asio.hpp>
#include <iostream>
#include <string>


using tcp = boost::asio::ip::tcp;

std::string GetData(tcp::socket &socket) {

	boost::asio::streambuf Buffer;

	boost::asio::async_read(socket, Buffer, "\n");
	std::string data = boost::asio::buffer_cast<const char*>(Buffer.data());
	return data;

}

void SendData(tcp::socket &socket, const std::string &msg) {

	boost::asio::write(socket, boost::asio::buffer(msg + "\n"));

}


int main(int argc, char* arcv[]) {

	boost::asio::io_service service;

	// Listening new connection
	tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 9999));

	tcp::socket socket(service);
	

	// waiting on connection
	acceptor.accept(socket);

	std::string name = GetData(socket);
	name.pop_back();

	std::string response, reply;
	reply = "Hello " + name + "!";
	std::cout << "Server: " << reply << std::endl;
	SendData(socket, reply);

	while (true) {
		response = GetData(socket);
		response.pop_back();

		if (response == "exit") break;

		std::cout << name << ": " << response << std::endl;;

		std::cout << "Server: ";  std::getline(std::cin, reply);
		SendData(socket, reply);
		if (reply == "exit") break;
	}

	return 0;
}