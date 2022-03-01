#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>

#include <sstream>
#include <vector>

#include "User.hpp"

class Message {
	public:
		Message(std::string, const User& );
		Message( const Message &rhs );
		~Message();

		const std::string				&getPrefix() const;
		const std::string				&getCommand() const;
		const std::vector<std::string>	&getParamets() const;
		const std::string				&getName() const;
		void							setCommand( const std::string &command );

	private:
		Message();
		std::string 			 		_prefix;
		std::string						_name;
		std::vector<std::string>		_parameters;
		std::string 			 		_command;
		bool							_isPrefix;
		bool							_isCommand;
		std::vector<std::string>		_split ( const std::string &str, char delimeter );
		void							_parse ( std::string str, const User& );
		void							_parseUtility ( std::vector<std::string> );
		int							_check_s1 ( const std::string &str );
		int							_check_s2 ( const std::string &str );
};

#endif
