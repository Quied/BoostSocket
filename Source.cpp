#include "dependecies.h"
#include "ClientSynchronization.h"

int main() {

	std::vector<std::string> Clients{ "This", "Will", "Can" };

	boost::thread_group threads;

	for (auto& Client : Clients) {
		threads.create_thread(boost::bind(sync_echo, Client));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}

	threads.join_all();

}