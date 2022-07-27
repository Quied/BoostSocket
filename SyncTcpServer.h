#pragma once
#include <iostream>
#include <string>
#include <ostream>
#include "addition.h"

ClientArray Clients;
asio::io_service service;

struct Client : boost::enable_shared_from_this<Client> {
private:
	ip::tcp::socket _socket;
	enum { max_msg = 1024 };
	int _already_Read;
	char _buff[max_msg];
	bool _started;
	std::string _username;
	bool _client_changed;
	boost::posix_time::ptime _last_ping;

public:
	Client() : _socket(service), _started(false), _already_Read(0) {
		_last_ping = boost::posix_time::microsec_clock::local_time();
	}


	std::string Username() const {
		return _username;
	}

	void Answer_to_client() {
		try {

		}
		catch (const boost::system::error_code& er) {
			// std::cout << er.message() << std::endl;
			stop();
		}
		if (time_out()) {
			stop();
			std::cout << _username << "- no ping" << std::endl;
		}
	}

	void Set_client_changes() { _client_changed = true; }

	ip::tcp::socket& sock() { return _socket; }

	bool time_out() const {
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		long long ms = (now - _last_ping).total_milliseconds();
		return ms > 5000;
	}

	// close client connection
	void stop() {
		boost::system::error_code er;
		_socket.close(er);
	}

private:

	void read_request() {
		if (_socket.available()) {
			_already_Read += _socket.read_some(
				asio::buffer(_buff + _already_Read, max_msg - _already_Read));
		}
	}

	void procces_request() {
		bool found_enter = std::find(_buff, _buff + _already_Read, '\n')
			< _buff + _already_Read;
		if (!found_enter)
			return; // message is not full
		// process the msg
		_last_ping = boost::posix_time::microsec_clock::local_time();
		size_t pos = std::find(_buff, _buff + _already_Read, '\n') - _buff;
		std::string msg(_buff, pos);
		std::copy(_buff + _already_Read, _buff + max_msg, _buff);
		_already_Read -= pos + 1;

		if (msg.find("login ") == 0) on_login(msg);
		else if (msg.find("ping") == 0) on_ping();
		else if (msg.find("ask_clients") == 0) on_clients();
		else std::cerr << "invalid msg " << msg << std::endl;

	}

	void on_login(const std::string& msg) {
		std::istringstream in(msg);
		in >> _username >> _username;
		std::cout << _username << " loged in" << std::endl;
		write("Login ok");
		Update_Client_changed();
	}

	void on_ping() {
		write(_client_changed ? "ping client_list_changes\n" : "ping ok\n");
		_client_changed = false;
	}
	void on_clients() {
		std::string msg;
		{
			boost::recursive_mutex::scoped_lock lk(rm);
			for (ClientArray::iterator b = Clients.begin(), e = Clients.end(); b != e; ++b)
				msg += (*b)->Username() + " ";
		}
		write("clients " + msg + "\n");
	}

	void write(const std::string& msg) {
		_socket.write_some(asio::buffer(msg));
	}

	void Update_Client_changed() {
		boost::recursive_mutex::scoped_lock lk(rm);
		for (ClientArray::iterator b = Clients.begin(), e = Clients.end(); b != e; ++b)
			(*b)->Set_client_changes();
	}

	void accept_thread() {
		ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
		while (true) {
			Client_ptr new_(new Client);
			acceptor.accept(new_->sock());

			boost::recursive_mutex::scoped_lock lk(rm);
			Clients.push_back(new_);
		}
	}

	// Process dialog msg with clients
	void handle_client_thread() {
		while (true) {
			boost::this_thread::sleep(boost::chrono::milliseconds(1));
			boost::recursive_mutex::scoped_lock lk(rm);

			for (ClientArray::iterator b = Clients.begin(), e = Clients.end(); b != e; ++b)
				(*b)->Answer_to_client();

			// remove clients that timed out
			Clients.erase(std::remove_if(Clients.begin(), Clients.end(),
				boost::bind(&Client::time_out, _1)), Clients.end());
		}
	}

};
