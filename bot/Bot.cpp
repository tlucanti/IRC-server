#include "Bot.hpp"
#include "User.hpp"
#include "Message.hpp"
#include <fstream>

const std::string toUpper(const std::string &str)
{
	std::string tmp(str.length(), 0);

	for (size_t i = 0; i < str.length(); ++i)
		tmp[i] = std::toupper(str[i]);
	return (tmp);
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *)sa)->sin_addr);
	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

Bot::Bot(const char *host, const char *port, const char* bname, const char* pass)
	: _hostname(host),
	  _port(port),
	  _nick(bname),
	  _delimeter("\r\n"),
	  _pass(pass)
{
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(_hostname, _port, &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddrinfo: " << stderr << std::endl;
		throw std::exception();
	}

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((_sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			std::cerr << "client: socket" << std::endl;
			continue;
		}

		if (connect(_sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(_sockfd);
			std::cerr << "client: connect" << std::endl;
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		std::cerr  << "client: failed to connectn"  << std::endl;
		throw std::exception();
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
	std::cerr  << "client: connecting to " << s << std::endl;

	freeaddrinfo(servinfo);
}

void Bot::run()
{
	std::string bot_buf;
	int ban_flag = 0;

	bot_buf = "PASS " + _pass + std::string("\r\n") +
		"NICK " + std::string(_nick) + std::string("\r\n") +
		"USER myusernaaame * * *" + std::string("\r\n");
	_send(bot_buf);
	User usr;
	usr.setNickname("bot");
	while (21)
	{
		try
		{
			_recv(bot_buf);
		}
		catch (const std::exception &e)
		{
			close(_sockfd);
			return;
		}

		std::string msg_str;
		std::string cmd_str;
		Message msg(bot_buf, usr);

		if (toUpper(msg.getCommand()) == "INVITE")
		{
			msg_str = "JOIN ";
			if (msg.getParamets().size() == 2)
			{
				msg_str += msg.getParamets()[1];
				_send(msg_str);
			}
			else
				continue;
		}
		else if (toUpper(msg.getCommand()) == "PRIVMSG")
		{
			cmd_str = "PRIVMSG ";
			if (!msg.getParamets().empty())
			{
				if (ban_flag == 1)
				{
					std::cout << " >>> BAN USER <<<\n";
					ban_flag = 0;
					std::string _msg = _printMsg(msg.getName(), "txt_files/ban.txt") +
						_ban(msg.getName(), msg.getParamets().at(0));
					::send(_sockfd, _msg.c_str(), _msg.size(), 0);
					//BAN CHANALE_NAME USER_NAME : why?
				}
				else if (msg.getParamets().back().find("idiot") != std::string::npos ||
					msg.getParamets().back().find("div") != std::string::npos ||
					msg.getParamets().back().find("loser") != std::string::npos ||
					msg.getParamets().back().find("arse") != std::string::npos ||
					msg.getParamets().back().find("bloody") != std::string::npos ||
					msg.getParamets().back().find("bugger") != std::string::npos ||
					msg.getParamets().back().find("cow") != std::string::npos ||
					msg.getParamets().back().find("bullshit") != std::string::npos ||
					msg.getParamets().back().find("arsehole") != std::string::npos)
				{
					std::cout << " >>> KICK USER <<< \n";
					ban_flag = 1;
					std::string _msg = _printMsg(msg.getName(), "txt_files/kick.txt") +
						_kick(msg.getName(), msg.getParamets().at(0));
					std::cout << "sending <" << _msg << ">\n";
					::send(_sockfd, _msg.c_str(), _msg.size(), 0);
					//KICK CHANALE_NAME USER_NAME : why?
				}
			}
			else
				continue;
		}
		else if (toUpper(msg.getCommand()) == "PING")
		{
			std::string _msg = "PONG :" + msg.getParamets().back() + "\r\n";
			::send(_sockfd, _msg.c_str(), _msg.size(), 0);
		}
	}
}

bool Bot::_recv(std::string &status)
{
	char    c_buf[BUF_SIZE];
	size_t  bytesLeft;
	int     bytes = 1;
	bool    res;

	status.clear();
	while (status.find(_delimeter) == std::string::npos && _recvBuffer.size() + status.size() < sizeof(c_buf))
	{
		memset(c_buf, 0, sizeof(c_buf));
		bytes = recv(_sockfd, c_buf, sizeof(c_buf) - 1 - (_recvBuffer.size() + status.size()), MSG_PEEK);
		if (bytes < 0)
		{
			if (errno == EAGAIN)
			{
				_recvBuffer += status;
				return (false);
			}
			std::cerr << strerror(errno);
			throw std::exception();
		}
		if (bytes == 0)
			throw std::exception();

		bytesLeft = std::string(c_buf).find(_delimeter);
		if (bytesLeft == std::string::npos)
			bytesLeft = bytes;
		else
			bytesLeft += _delimeter.length();
		while (bytesLeft > 0)
		{
			memset(c_buf, 0, sizeof(c_buf));
			bytes = recv(_sockfd, c_buf, bytesLeft, 0);
			if (bytes < 0)
			{
				if (errno == EAGAIN)
				{
					_recvBuffer += status;
					return (false);
				}
				std::cerr << strerror(errno);
				throw std::exception();
			}
			if (bytes == 0)
				throw std::exception();
			bytesLeft -= bytes;
			status += c_buf;
		}
	}
	if (status.find(_delimeter) == std::string::npos)
		res = false;
	else
		res = true;
	_recvBuffer += status;
	status = _recvBuffer;
	_recvBuffer.clear();
	status.erase(status.end() - _delimeter.length(), status.end());
	std::cout << "Done \"" << status << "\"" << std::endl;
	return (res);
}

bool Bot::_send(const std::string &status)
{
	std::string buf_delim(status);
	int total = 0;
	int bytesLeft;
	int bytes;

	if (buf_delim.find(_delimeter) != buf_delim.length() - _delimeter.length())
		buf_delim += _delimeter;
	bytesLeft = buf_delim.length();
	while (bytesLeft > 0)
	{
		bytes = send(_sockfd, buf_delim.c_str() + total, bytesLeft, 0);
		if (bytes < 0)
		{
			if (errno == EAGAIN)
			{
				_sendBuffer = buf_delim.c_str() + total;
				return (false);
			}
			std::cerr << strerror(errno);
			break;
		}
		total += bytes;
		bytesLeft -= bytes;
	}
	return (bytes == -1 ? false : true);
}

std::string Bot::_printMsg(const std::string &name, const char *fname) const
{
	std::ifstream fin(fname);
	if (not fin.is_open() or fin.fail())
	{
		std::cout << "[ERROR]: cannot open `" << fname << "` file\n";
		return "";
	}
	std::string start;
	{
		std::stringstream ss;
		ss << "PRIVMSG " << name << " :";
		start = ss.str();
	}
	std::stringstream ss;
	while (fin)
	{
		std::string line;
		std::getline(fin, line);
		if (line.empty())
			break ;
		ss << start << line << "\r\n";
	}
	return ss.str();
}

std::string Bot::_kick(const std::string &name, const std::string &chan) const
{
	std::stringstream ss;
	ss << "KICK " << chan << ' ' << name << " :bad language\n\r";
	return ss.str();
}

std::string Bot::_ban(const std::string &name, const std::string &chan) const
{
	std::stringstream ss;
	ss << "MODE " << chan << " +b " << name << "\n\r";
	return ss.str() + _kick(name, chan);
}
