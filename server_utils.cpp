/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 13:32:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/04 13:36:00 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

namespace tlucanti
{
	__NORET void server_start(Server &server)
	{
		while (true)
		{
			Socket new_cli = server.accept();
			if (new_cli != Socket::nil) {
				std::cout << "add new client " << new_cli.get_sock() << std::endl;
				server.add_client(new_cli);
			}
			Socket client = server.poll();
			if (client == Socket::nil)
				continue;
			std::string cli_data = tlucanti::recv(client);
			std::cout << "data from client " << client.get_sock() << " (" << cli_data.size() << "): <" << cli_data << ">\n";
		}
	}
}
