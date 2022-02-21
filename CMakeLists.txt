cmake_minimum_required(VERSION 3.21)
project(irc)

set(CMAKE_CXX_STANDARD 23)

add_executable(irc
	inc/lexical_cast.hpp
	inc/SocketException.hpp
	inc/defs.h
	inc/IRCException.hpp
	inc/ServerException.hpp
	inc/Server.hpp
	inc/global.h
	inc/Database.hpp
	inc/ITarget.hpp
	inc/User.hpp
	inc/Socket.hpp
	inc/Channel.hpp
	inc/InviteNode.hpp
	inc/IRCcodes.h
	inc/Color.hpp
	inc/IRCrpl.hpp
	inc/IRCParser.hpp
	inc/Mutex.hpp
	inc/Thread.hpp
	inc/IRCParserException.hpp
	inc/parser_utils.hpp

	src/IRCParser.cpp
	src/Mutex.cpp
	src/IRCParser_dcc_compose.cpp
	src/Socket.cpp
	src/Channel.cpp
	src/IRCParser_compose.cpp
	src/server_utils.cpp
	src/IRCcodes.cpp
	src/IRCParser_exec.cpp
	src/Database.cpp
	src/IRCParser_parse.cpp
	src/User.cpp
	src/IRCException.cpp
	src/main.cpp
	src/Server.cpp
)

add_executable(test
	test/test.cpp

	src/IRCcodes.cpp

	inc/defs.h
	inc/IRCrpl.hpp
	inc/IRCcodes.h
)
