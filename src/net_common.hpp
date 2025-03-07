#pragma once

#include <memory>
#include <vector>
#include <thread>
#include <iostream>
#include <deque>
#include <mutex>
#include <optional>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <boost/asio.hpp>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
