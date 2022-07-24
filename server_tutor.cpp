#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/ts/internet.hpp>
#include <thread>
#include <chrono>


namespace ip = boost::asio::ip;
namespace asio = boost::asio;

std::vector<char> vBuffer(20 * 1024);

void GrabData(ip::tcp::socket &socket){
		socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t len){
		if(!ec){
		std::cout << "\n\nRead: " << len << " Bytes\n\n";

				for(int i = 0; i < len; i++)
						std::cout << vBuffer[i] << std::endl;
			
				GrabData(socket);
			}
		});
}

int main(){
	
		// information about error
		boost::system::error_code ec;
		
		//platform specific interface
		asio::io_context context;

		// get address
		asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);
		// create socket, 
		asio::ip::tcp::socket socket(context);
	
		// Try connect
		socket.connect(endpoint, ec);

		if(!ec) std::cout << "Connected" << std::endl;
		else{std::cout << "Failed: " << ec.message() << std::endl;}

		if(socket.is_open()){
				GrabData(socket);
				std::string Request = 
						"GET /index.html HTTP/1.1\r\n"
						"Host: example.com\r\n"
						"Connection: close\r\n\r\n";	

		socket.write_some(asio::buffer(Request.data(), Request.size()), ec);

		GrabData(socket);
		
		}


}
