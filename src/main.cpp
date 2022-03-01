/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 16:02:00 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/07 20:58:18 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Color.hpp"
#include "../inc/Database.hpp"

#include <string>

const int tlucanti::Socket::READ_SIZE = 64;
const int tlucanti::Server::WAIT_TIME = 500;
namespace tlucanti
{
	tlucanti::Database database;
	sig_atomic_t server_int;

	const char *server_name = "__TLUCANTI__";
	const char *server_address = "0.0.0.0";
	const char *server_version = "1.0";
	const char *server_source_link = "https://github.com/tlucanti/irc";

	std::string server_oper_login;
	std::string server_oper_password;
	std::string server_password;
	std::string server_begining;
	unsigned short	server_port;
	unsigned int	invite_expiration = 60;
	unsigned int	ping_expiration = 255;

	const unsigned int channel_max_users = 500;
	const unsigned int chanel_max_name_len = 50;
	const unsigned int channel_max_topic_len = 300;
	const unsigned int user_max_channels = 32;
	const unsigned int user_max_nick_len = 20;

	Mutex ping_mutex;

	namespace IRC {
		const std::string endl = "\r\n";
	}
}

__WUR
std::string
get_current_time()
{
	const char *month[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September",
						   "October", "November", "December"};
	char buf[100] = {};
	std::string ret;

	time_t now = time(nullptr);
	struct tm tstruct = *localtime(&now);
	bzero(buf, sizeof(buf));
	strftime(buf, sizeof(buf) / sizeof(char), "%d", &tstruct);
	ret += buf;
	bzero(buf, sizeof(buf));
	strftime(buf, sizeof(buf) / sizeof(char), "%m", &tstruct);
	ret += ' ';
	ret += month[tlucanti::lexical_cast<int, 10>(buf)];
	bzero(buf, sizeof(buf));
	strftime(buf, sizeof(buf) / sizeof(char), "%d", &tstruct);
	ret += ' ';
	ret += buf;
	bzero(buf, sizeof(buf));
	strftime(buf, sizeof(buf) / sizeof(char), "%Y", &tstruct);
	ret += ' ';
	ret += buf;
	ret += " at ";
	bzero(buf, sizeof(buf));
	strftime(buf, sizeof(buf) / sizeof(char), "%X", &tstruct);
	ret += buf;
	return ret;
}

void check_args(int argc, const char *const *argv)
{
	if (argc == 2 and std::string(argv[1]) == "--help")
	{
		tlucanti::cout << "[b][INFO] [p]c++[w] irc webserver[]\n";
		tlucanti::cout << "[p] usage:[c] ./ircserv [y]PORT PASSWORD[]\n";
		throw tlucanti::Exception("server", "restart server with arguments");
	}
	else if (argc < 3)
		throw tlucanti::Exception("server", "need more arguments",
			"type --help for more information");
	else if (argc > 3)
		throw tlucanti::Exception("server", "too many arguments",
			"type --help for more information");
	else
	{
		tlucanti::server_password = argv[2];
		tlucanti::server_begining = get_current_time();
		try {
			tlucanti::server_port = tlucanti::lexical_cast<unsigned short, 10>(argv[1]);
		} catch(tlucanti::bad_lexical_cast &exc)
		{
			throw tlucanti::Exception("lexical_cast", exc.what());
		}
		tlucanti::server_int = 0;
	}
	{
		const char *_oper_login = getenv("OPER_LOGIN");
		const char *_oper_pass = getenv("OPER_PASS");
		if (_oper_login == nullptr)
		{
			tlucanti::WARN("server", "operator login not found in environment variable `OPER_LOGIN`");
			_oper_login = "";
		}
		if (_oper_pass == nullptr)
		{
			tlucanti::WARN("server", "operator password not found in environment variable `OPER_PASS`");
			_oper_pass = "";
		}
		tlucanti::server_oper_login = _oper_login;
		tlucanti::server_oper_password = _oper_pass;
	}
}

int main(int argc, char *const *argv)
{
	try {
		check_args(argc, argv);
	} catch (std::exception &exc) {
		tlucanti::cout << exc.what() << tlucanti::endl;
		return 1;
	}
#ifndef __DEBUG
	try
	{
#endif /* __DEBUG */
		tlucanti::Server server(tlucanti::server_address, tlucanti::server_port);
		tlucanti::cout << "[w]server started at address: [" << tlucanti::server_address << "/" << argv[1] << "][]\n";
		tlucanti::server_start(server);
		if (tlucanti::server_int == 'r')
			execv(argv[0], argv);
		else if (tlucanti::server_int == 'q')
			return 0;
#ifndef __DEBUG
	}
	catch (std::exception &exc) {
		tlucanti::cout << "[r][FATAL]: [r]" << exc << tlucanti::endl;
		tlucanti::cout << "[r][*** FATAL ERROR ***]: server shutting down[]\n";
		try {
			tlucanti::database.send_to_all(tlucanti::IRC::compose_message(nullptr, "NOTICE", '*',
				"*** SERVER FATAL ERROR *** SERVER SHUTTING DOWN"));
		} catch (std::exception &)
		{
			std::cout << tlucanti::color::r << "[*** FATAL ERROR ***]: cannot send collapsing message to users";
			std::cout << tlucanti::color::s << "\n";
		}
		tlucanti::database.collapse();
		return 1;
	}
#endif /* __DEBUG */
}
