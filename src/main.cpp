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
	const tlucanti::color cout;
	tlucanti::Database database;
	sig_atomic_t server_run;

	const char *server_name = "__TLUCANTI__";
	const char *server_address = "0.0.0.0";
	const char *server_version = "1.0";
	const char *server_oper_login = "tlucanti";
	const char *server_oper_password = "oper";

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
		const std::string endl = "\n";
	}
}

__WUR
std::string
get_current_time()
{
	const char *month[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September",
						   "October", "November", "December"};
	const char *day[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	char buf[100] = {};
	std::string ret;

	time_t now = time(nullptr);
	struct tm tstruct = *localtime(&now);
	bzero(buf, sizeof(buf));
	strftime(buf, sizeof(buf) / sizeof(char), "%d", &tstruct);
	ret += day[tlucanti::lexical_cast<int>(buf)];
	bzero(buf, sizeof(buf));
	strftime(buf, sizeof(buf) / sizeof(char), "%m", &tstruct);
	ret += ' ';
	ret += month[tlucanti::lexical_cast<int>(buf)];
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
	if (argc == 2 and std::string(argv[2]) == "--help")
	{
		tlucanti::cout << "[b][INFO] [p]c++[w] irc webserver[]\n";
		tlucanti::cout << "[p] usage:[c] ./ircserv [y]PORT PASSWORD[]\n";
		throw tlucanti::IRCException("server", "restart server with arguments");
	}
	else if (argc < 3)
		throw tlucanti::IRCException("server", "need more arguments",
			"type --help for more information");
	else if (argc > 3)
		throw tlucanti::IRCException("server", "too many arguments",
			"type --help for more information");
	else
	{
		tlucanti::server_password = argv[2];
		tlucanti::server_begining = get_current_time();
		try {
			tlucanti::server_port = tlucanti::lexical_cast<unsigned short>(argv[1]);
		} catch(tlucanti::bad_lexical_cast &exc)
		{
			throw tlucanti::IRCException("lexical_cast", exc.what());
		}
		tlucanti::server_run = 0;
	}
}

int main(int argc, char *const *argv)
{
//	try
	{
		check_args(argc, argv);
		tlucanti::Server server(tlucanti::server_address, tlucanti::server_port);
		tlucanti::cout << "[w]server started at address: [" << tlucanti::server_address << "/" << argv[1] << "][]\n";
		tlucanti::server_start(server);
		if (tlucanti::server_run == 'r')
			execv(argv[0], argv);
		else if (tlucanti::server_run == 'q')
			return 0;
	}
//	catch (std::exception &exc) {
//		tlucanti::cout << exc;
//	}
}
