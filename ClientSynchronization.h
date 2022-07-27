#pragma once

#include "dependecies.h"

using error = boost::system::error_code;
boost::asio::io_service service;

const int buf_size = 1024;

size_t read_complete(char* buf, const error& er, size_t bytes) {

	if(er){
		std::cout << er.message() << std::endl;
		return 0;
	}
	
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

ip::tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), 8001);

void sync_echo(std::string msg) {
	msg += '\n';
	ip::tcp::socket socket(service);
	socket.connect(endpoint);
	socket.write_some(asio::buffer(msg));
	char buf[buf_size];
	int bytes = asio::read(socket, asio::buffer(buf), boost::bind(read_complete, buf, _1, _2));
	std::string copy(buf, bytes - 1);
	msg = msg.substr(0, msg.size() - 1);
	std::cout << "server echoed: " << msg << ": " << (copy == msg ? "Ok" : "Fail") << std::endl;
	socket.close();
}


