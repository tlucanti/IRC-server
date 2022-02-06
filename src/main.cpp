/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 16:02:00 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:48 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Color.hpp"

const int tlucanti::Socket::READ_SIZE = 64;
const int tlucanti::Server::WAIT_TIME = 500;
namespace tlucanti
{
	const tlucanti::color cout;

	std::string server_name;
	std::string server_password;
	unsigned short server_port;
}

void check_args(int argc, const char **argv)
{
	if (argc == 2 and std::string(argv[2]) == "--help")
	{
		tlucanti::cout << "[b][INFO] [p]c++[w] irc webserver[]\n";
		tlucanti::cout << "[p] usage:[c] ./ircserv [y]PORT PASSWORD[]\n";
		throw tlucanti::IRCException("serve", "restart server with arguments");
	}
	else if (argc < 3)
		throw tlucanti::IRCException("server", "need more arguments",
			"type --help for more information");
	else if (argc > 3)
		throw tlucanti::IRCException("server", "too many arguments",
			"type --help for more information");
	else
	{
		tlucanti::server_name = "__TLUCANTI_SERVER__";
		tlucanti::server_password = argv[2];
		tlucanti::server_port = atoi(argv[1]); // make normal string -> int
	}
}

int main(int argc, const char **argv)
{
//	try {
		check_args(argc, argv);
		tlucanti::Server server("0.0.0.0", tlucanti::server_port);
		tlucanti::cout << "[w]server started at 0.0.0.0:" << argv[1] << "[]\n";
		tlucanti::server_start(server);
//	} catch (std::exception &exc) {
//		tlucanti::cout << exc;
//	}
}
