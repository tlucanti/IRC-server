cmake_minimum_required(VERSION 3.21)
project(irc)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS -D__DEBUG)

include_directories(inc)
link_directories(.)

add_executable(irc
	Bot.cpp
	Bot.hpp
	Message.cpp
	Message.hpp
	User.cpp
	User.hpp
	main.cpp
)
