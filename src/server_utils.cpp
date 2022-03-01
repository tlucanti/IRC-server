/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 13:32:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/23 22:59:32 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <mutex>

#include "../inc/Server.hpp"
#include "../inc/IRCParser.hpp"
#include "../inc/Thread.hpp"
#include "../inc/Color.hpp"

namespace tlucanti
{
	extern Database database;
	extern sig_atomic_t server_int;
	std::string make_response(const Socket &client, const std::string &request);
	void main_thread(Server &server);
	void *invite_thread(void *);
	void *ping_thread(void *);
}

namespace tlucanti
{
	void server_start(Server &server)
	{
		Thread inv_daemon(reinterpret_cast<void *(*)(void *)>(invite_thread), nullptr);
		Thread ping_daemon(reinterpret_cast<void *(*)(void *)>(ping_thread), nullptr);
		main_thread(server);
		inv_daemon.join();
		ping_daemon.join();
	}

	void main_thread(Server &server)
	{
		std::map<Socket, std::string> cached;
		while (not tlucanti::server_int)
		{
			try {
				Socket new_cli = server.accept();
				if (new_cli != Socket::nil)
				{
					std::cout << "new client (" << new_cli.get_sock() <<
						") connected: " << new_cli.get_address() << ':' <<
						new_cli.get_port() << std::endl;
					server.add_client(new_cli);
					database.add_client(new_cli);
				}
				Socket client = server.poll();
				if (client == Socket::nil)
					continue;
				std::string request;
				try {
					request = cached[client] + client.recv();
				} catch (SocketException &exc) {
#ifdef __DEBUG
					tlucanti::cout << exc.what() << tlucanti::endl;
#endif /* __DEBUG */
					User *usr = database[client];
					usr->send_to_channels(IRC::compose_message(usr->compose(), "QUIT", nullptr, "Lost connection"));
					database.remove_client(client);
				}
				if (request.back() != '\n' and request.back() != '\r')
				{
					cached[client] = request;
					continue ;
				}
				else
					cached.erase(client);
				std::vector<std::string> commands;
				split(request, commands, '\n');
				std::vector<std::string>::iterator it=commands.begin();
				for (; it != commands.end(); ++it)
				{
					if (it->empty())
						continue ;

					std::string response = make_response(client, *it);
					if (response.empty())
						ABORT("empty response", "");
					if (response != "OK")
						client.send(response);
//#ifdef __DEBUG
//					if (response == "OK")
//						client.send(response + IRC::endl);
//#endif /* __DEBUG */
				}
			}
			catch (Exception &exc)
			{
				tlucanti::cout << exc.what() << "\n";
			}
		}
	}

	void *invite_thread(void *)
	{
		while (not tlucanti::server_int)
		{
			Database::invite_table_type::iterator it = database.invite_table.begin();
			for (; it != database.invite_table.end(); )
			{
				if (time(nullptr) > it->second)
				{
					Database::invite_table_type::iterator next = it;
					++next;
					database.remove_invite(it->first);
					it = next;
				}
				else
					++it;
			}
			sleep(1);
		}
		return nullptr;
	}

	void *ping_thread(void *)
	{
		int wait = 0;
		while (not tlucanti::server_int)
		{
			++wait;
			sleep(1);
			if (wait < 20)
				continue ;
			wait = 0;
			Database::sock_hashmap_type::const_iterator it = database.sock_access.begin();
			ping_mutex.lock();
			for (; it != database.sock_access.end(); ++it)
			{
				User *user = it->second;
				if (not user->ping_waiting and user->has_mode("reg+") and time(nullptr) > user->last_ping + 180)
					user->do_ping();
				else if (user->ping_waiting and time(nullptr) > user->last_ping + tlucanti::ping_expiration)
				{
					std::string message = "Ping timeout: " + numeric_cast<std::string, 10, true>(255) + " seconds";
					user->send_to_channels(IRC::compose_message(user->compose(), "QUIT", nullptr, message));
					user->send_message(IRC::ERROR(message));
					database.remove_client(*(it->second));
				}
			}
			ping_mutex.unlock();
		}
		return nullptr;
	}

	std::string make_response(const Socket &client, const std::string &request)
	{
		ping_mutex.lock();
		std::string response;
		try {
			response = IRCParser(request).exec(client);
		} catch (IRCParserException &exc) {
			response = exc.what();
		}
		ping_mutex.unlock();
		return response;
	}
}
