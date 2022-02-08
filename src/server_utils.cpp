/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 13:32:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:52 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "../inc/Server.hpp"
#include "../inc/Database.hpp"
#include "../inc/IRCParser.hpp"

namespace tlucanti
{
	extern Database database;
	std::string make_response(const Socket &client, const std::string &request);
}

namespace tlucanti
{
	__NORET void server_start(Server &server)
	{
		while (true)
		{
			Socket new_cli = server.accept();
			if (new_cli != Socket::nil)
			{
				std::cout << "add new client " << new_cli.get_sock() << std::endl;
				server.add_client(new_cli);
				database.add_client(new_cli);
			}
			Socket client = server.poll();
			if (client == Socket::nil)
				continue;
			std::string request = client.recv();
			std::vector<std::string> commands;
			split(request, commands, '\n');
			for (std::vector<std::string>::iterator it=commands.begin(); it != commands.end(); ++it)
			{
				if (it->empty())
					continue ;

				std::cout << "data from client " << client.get_sock() << " (" << it->size() << "): <" << it->substr(0, it->size() - 1) << ">\n";
				std::string response = make_response(client, *it);
				if (not response.empty())
					client.send(response);
			}
		}
	}

	std::string make_response(const Socket &client, const std::string &request)
	{

		try
		{
			return IRCParser(request).exec(client);
		} catch (IRCParserException &exc) {
			return exc.what();
		}
	}
}
