#include <iostream>
#include <vector>
#include <thread>
#include "boost/asio.hpp"

namespace bst_a = boost::asio;
namespace bst_ip = boost::asio::ip;

void Show(const boost::system::error_code &err){
		std::cout << "Show" << std::endl;
		std::cout << "boost error: " <<err.what() << std::endl;
}

int main(int argc, char *argv[]){

		try{
		if(argc != 2){
				std::cout << "Client host" << std::endl;
				return 1;
			}
		

		bst_a::io_service io;
		bst_ip::tcp::resolver resolver(io);
		bst_ip::tcp::resolver::query query(argv[1], "daytime");
		bst_ip::tcp::resolver::iterator endpnt_iterator = resolver.resolve(query);

		bst_ip::tcp::socket socket(io);
		bst_a::connect(socket, endpnt_iterator);

		for(;;){
		std::vector<char> buff;
		boost::system::error_code error;

		size_t len = socket.read_some(bst_a::buffer(buff), error);
		
		if(error == bst_a::error::eof) break;
		else if(error) throw boost::system::system_error(error);

		std::cout.write(buff.data(), len);

		 }
		}
		catch(const std::exception &ex){
				std::cout << ex.what() << std::endl;
		}
		


}
