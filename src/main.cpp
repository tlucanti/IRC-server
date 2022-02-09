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

const int tlucanti::Socket::READ_SIZE = 64;
const int tlucanti::Server::WAIT_TIME = 500;
namespace tlucanti
{
	const tlucanti::color cout;
	tlucanti::Database database;

	const char *server_name = "__TLUCANTI__";
	const char *server_address = "0.0.0.0";
	const char *server_version = "1.0";
	const char *server_oper_login = "tlucanti";
	const char *server_oper_password = "oper";

	std::string server_password;
	std::string server_begining;
	unsigned short server_port;

	const unsigned int channel_max_users = 500;
	const unsigned int chanel_max_name_len = 50;
	const unsigned int channel_max_topic_len = 300;
	const unsigned int user_max_channels = 32;
	const unsigned int user_max_nick_len = 20;

	namespace IRC
	{
		std::string endl = "\n";
	}
}

void check_args(int argc, const char **argv)
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
		tlucanti::server_begining = tlucanti::get_current_time();
		tlucanti::server_port = atoi(argv[1]); // make normal string -> int
	}
}

int main(int argc, const char **argv)
{
//	try {
		check_args(argc, argv);
		tlucanti::Server server(tlucanti::server_address, tlucanti::server_port);
		tlucanti::cout << "[w]server started at address: [" << tlucanti::server_address << "/" << argv[1] << "][]\n";
		tlucanti::server_start(server);
//	} catch (std::exception &exc) {
//		tlucanti::cout << exc;
//	}
}
