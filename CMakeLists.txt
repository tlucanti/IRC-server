cmake_minimum_required(VERSION 3.21)
project(irc)

set(CMAKE_CXX_STANDARD 23)

add_executable(irc
	IRCException.cpp
	Server.cpp
	Socket.cpp
	main.cpp
	server_utils.cpp
	socket_utils.cpp
	Color.hpp
	IRCException.hpp
	Server.hpp
	ServerException.hpp
	Socket.hpp
	SocketException.hpp
)
