#include <boost/asio.hpp>
#include <iostream>
#include <string>


using tcp = boost::asio::ip::tcp;


std::string GetData(tcp::socket& socket) {

	boost::asio::streambuf Buffer;

	boost::asio::async_read(socket, Buffer, "\n");
	std::string data = boost::asio::buffer_cast<const char*>(Buffer.data());
	return data;

}

void SendData(tcp::socket& socket, const std::string& msg) {

	boost::asio::write(socket, boost::asio::buffer(msg + "\n"));

}

int main(char* argv[], int argc) {

	boost::asio::io_service service;

	tcp::socket socket(service);

	socket.connect(tcp::endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), 9999));

	std::cout << "Name: ";
	std::string name, reply, response;
	getline(std::cin, name);

	// Send name
	SendData(socket, name);

	while(true){
	
		response = GetData(socket);
		response.pop_back();

		if (response == "exit") {
			std::cout << "Connection lost" << std::endl;
			break;
		}
		std::cout << "Server: " << response << std::endl;

		std::cout << name << ": ";
		getline(std::cin, reply);
		SendData(socket, reply);

		if (reply == "exit") break;

	}
	return 0;
}