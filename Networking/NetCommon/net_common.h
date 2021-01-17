#pragma once

#include<memory>
#include<thread>
#include<mutex>
#include<deque>
#include<optional>
#include<vector>
#include<iostream>
#include<algorithm>
#include<chrono>
#include<cstdint>

#ifdef _WIN32 //For windows, clear warnings
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE //use ASIO without boost

#include<asio.hpp>
#include<asio/ts/buffer.hpp> //Handles moving memory
#include<asio/ts/internet.hpp> //ASIO prepares for work with network communication
