cmake_minimum_required(VERSION 3.21)
project(irc)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-D__DEBUG -Wall -Wextra -Werror -D__CMAKE")

include_directories(inc)
link_directories(.)

add_executable(irc
	inc/lexical_cast.hpp
	inc/SocketException.hpp
	inc/defs.h
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
	inc/Exception.hpp

	src/IRCParser.cpp
	src/IRCParser_dcc_compose.cpp
	src/Exception.cpp
	src/Channel.cpp
	src/IRCParser_compose.cpp
	src/server_utils.cpp
	src/IRCcodes.cpp
	src/Color.cpp
	src/IRCParser_exec.cpp
	src/Database.cpp
	src/IRCParser_parse.cpp
	src/User.cpp
	src/main.cpp

	webcore/src/Mutex.cpp
	webcore/src/Socket.cpp
	webcore/src/Server.cpp

	webcore/inc/SocketException.hpp
	webcore/inc/defs.h
	webcore/inc/Exception.hpp.orig
	webcore/inc/ServerException.hpp
	webcore/inc/Server.hpp
	webcore/inc/Socket.hpp
	webcore/inc/Mutex.hpp
	webcore/inc/Thread.hpp
)

target_link_libraries(irc
	webcore
)

add_executable(test
	test/test.cpp

	src/IRCcodes.cpp

	inc/defs.h
	inc/IRCrpl.hpp
	inc/IRCcodes.h
)
