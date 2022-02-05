cmake_minimum_required(VERSION 3.21)
project(irc)

set(CMAKE_CXX_STANDARD 23)

add_executable(irc
	inc/SocketException.hpp
	inc/defs.h
	inc/IRCException.hpp
	inc/ServerException.hpp
	inc/Server.hpp
	inc/Database.hpp
	inc/User.hpp
	inc/Socket.hpp
	inc/Channel.hpp
	inc/IRCcodes.h
	inc/Color.hpp
	inc/IRCParser.hpp
	inc/IRCParserException.hpp

	src/IRCParser.cpp
	src/Socket.cpp
	src/Channel.cpp
	src/server_utils.cpp
	src/socket_utils.cpp
	src/Database.cpp
	src/IRCException.cpp
	src/main.cpp
	src/Server.cpp
)
