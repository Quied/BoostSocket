#pragma once

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <cstdio>
#endif

#include <iostream>
#include <cstring>
#include <vector>

#include "boost/asio.hpp"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace asio = boost::asio;
namespace ip = boost::asio::ip;