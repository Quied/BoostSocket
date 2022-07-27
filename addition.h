#pragma once

#ifdef WIN32
#define _WIN23_WINNT 0x0501
#include <stdio.h>
#endif

#include "dependecies.h"


struct Client;
using Client_ptr = boost::shared_ptr<Client>;
using ClientArray = std::vector<Client_ptr>;

boost::recursive_mutex rm;


void Update_Client_changed();

void Handler_Error(const boost::system::error_code& er) {
	 if(!er){ }
	 else {
		 std::cout << "Failed: " << er.message() << std::endl;
		 throw boost::system::system_error(er);
	 }
}


template<class ForwardIterator, class UnaryPredicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred);