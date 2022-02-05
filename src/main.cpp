/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 16:02:00 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:02:05 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Color.hpp"

const int tlucanti::Socket::READ_SIZE = 64;
const int tlucanti::Server::WAIT_TIME = 500;
namespace tlucanti { const tlucanti::color cout; }

int main()
{
//	try {
		tlucanti::Server server("0.0.0.0", 8080);
		tlucanti::server_start(server);
//	} catch (std::exception &exc) {
//		tlucanti::cout << exc;
//	}
}
