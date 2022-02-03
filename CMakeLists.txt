cmake_minimum_required(VERSION 3.21)
project(webserv)

set(CMAKE_CXX_STANDARD 23)

add_executable(webserv
	Server.cpp
	Socket.cpp
	SocketException.cpp
	main.cpp
	socket_utils.cpp
	Color.hpp
	Server.hpp
	Socket.hpp
	SocketException.hpp
)
