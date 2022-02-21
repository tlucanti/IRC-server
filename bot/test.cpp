/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 13:40:27 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/21 15:43:22 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "inc/IRCParserBot.hpp"

int main()
{
	while (true)
	{
		std::string line;
		std::getline(std::cin, line);
		try {
			tlucanti::IRCParserBot parser(line);
			parser.parse();
			std::cout << "parsed command: <" << line << ">\n";
			std::cout << "origin: <" << parser.get_origin() << ">\n";
			std::cout << "command: <" << parser.get_command() << ">\n";
			std::cout << "target: <" << parser.get_target() << ">\n";
			std::cout << "content: <" << parser.get_content() << ">\n";
		} catch (std::exception &exc) {
			std::cout << "[ERROR]: " << exc.what() << std::endl;
		}
	}
}
