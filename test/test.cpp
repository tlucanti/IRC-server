/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 17:34:03 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/16 23:24:23 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>

#include "../inc/defs.h"
#include "../inc/IRCrpl.hpp"
#include "../inc/IRCcodes.h"

void RPL_test(const std::string &resp) {
	for (char c : resp)
	{
		if (isprint(c))
			std::cout << c;
		else {
			std::cout << "\n\n\n!!!!!!! (" << (int) (c) << ") !!!!!!!!!\n\n\n";
		}
	}
	std::cout << std::endl;
}

using namespace tlucanti::IRC;

namespace tlucanti
{
	const char *server_name = "SERVER_NAME";
	unsigned short server_port = 9999;
	const char *server_address = "SERVER_ADDRESS";
	const char *server_version = "SERVER_VERSION";
	std::string server_begining = "SERVER_BEGINING";

	namespace IRC {
		std::string endl = "[ENDL]";
	}
}

struct RPL_test_Target
{
	__WUR bool is_oper(const std::string &name) const { return name == "USER1"; }
	__WUR bool is_voice(const std::string &name) const { return name == "USER2"; }
	__WUR std::string compose() const { return "COMPOSED_NAME"; }
	__WUR std::string get_username() const { return "COMPOSED_USERNAME"; }
	__WUR std::string get_realname() const { return "COMPOSED_REALNAME"; }
	__WUR std::string get_address() const { return "COMPOSE_ADDRESS"; }
	__WUR const RPL_test_Target &get_sock() const { return *this; }
};

std::ostream &operator <<(std::ostream &out, const RPL_test_Target &_tar) { out << "COMPOSED_OPERATOR"; return out; }

void test_rpl()
{
	const char *u1 = "USER1";
	const char *u2 = "USER2";
	const char *u3 = "USER3";
	std::list<const char **> NAMREPLY_user_list;
	NAMREPLY_user_list.push_back(&u1);
	NAMREPLY_user_list.push_back(&u2);
	NAMREPLY_user_list.push_back(&u3);
	RPL_test_Target NAMREPLY_tar;

	RPL_test(compose_message("FROM", "COMMAND", "TO", "MESSAGE"));
	RPL_test(ERROR("MESSAGE"));
	RPL_test(RPL_WELCOME("TARGET"));
	RPL_test(RPL_YOURHOST("TARGET"));
	RPL_test(RPL_CREATED("TARGET"));
	RPL_test(RPL_MYINFO("TARGET", "USER_MODES", "CHANNEL_MODES"));
	RPL_test(RPL_ISUPPORT("TARGET"));
	RPL_test(RPL_UMODEIS("TARGET", "MODES"));
	RPL_test(RPL_LUSERCLIENT("TARGET", 123, 456, 789));
	RPL_test(RPL_LUSEROP("TARGET", 123));
	RPL_test(RPL_LUSERCHANNELS("TARGET", 123));
	RPL_test(RPL_LUSERME("TARGET", 123, 456));
	RPL_test(RPL_LOCALUSERS("TARGET", 123, 456));
	RPL_test(RPL_GLOBALUSERS("TARGET", 123, 456));
	RPL_test(RPL_AWAY(NAMREPLY_tar, "RECIPIENT", "MESSAGE"));
	RPL_test(RPL_ENDOFWHO("TARGET", "WHO"));
	RPL_test(RPL_LISTSTART("TARGET"));
	RPL_test(RPL_LIST("TARGET", "CHANNEL", 999, "TOPIC"));
	RPL_test(RPL_LISTEND("TARGET"));
	RPL_test(RPL_CHANNELMODEIS("TARGET", "CHANNEL", "MODES", "ARGS"));
	RPL_test(RPL_CREATIONTIME("TARGET", "CHANNEL", "TIME"));
	RPL_test(RPL_NOTOPIC("TARGET", "CHANNEL"));
	RPL_test(RPL_TOPIC("TARGET", "CHANNEL", "TOPIC"));
	RPL_test(RPL_TOPICWHOTIME("TARGET", "CHANNEL", "AUTHOR", "TIME"));
	RPL_test(RPL_WHOREPLY("TARGET", NAMREPLY_tar, "CHANNEL"));
	RPL_test(RPL_NAMREPLY("TARGET", 'C', "CHANNEL", NAMREPLY_user_list,
		NAMREPLY_tar));
	RPL_test(RPL_ENDOFNAMES("TARGET", "CHANNEL"));
	RPL_test(ERR_NOMOTD("TARGET", "FNAME"));
	RPL_test(RPL_MOTD("TARGET", "../test/test_motd.txt"));
	RPL_test(RPL_MOTDSTART("TARGET"));
	RPL_test(RPL_ENDOFMOTD("TARGET"));
	RPL_test(RPL_YOUREOPER("TARGET", "MESSAGE"));
	RPL_test(ERR_NOSUCHNICK("TARGET", "RECIPIENT", "WHO"));
	RPL_test(ERR_NOSUCHCHANNEL("TARGET", "CHANNEL", "MESSAGE"));
	RPL_test(ERR_CANNOTSENDTOCHAN("TARGET", "CHANNEL", "MESSAGE"));
	RPL_test(ERR_TOOMANYCHANNELS("TARGET", "CHANNEL"));
	RPL_test(ERR_NORECIPIENT("TARGET", "CHANNEL"));
	RPL_test(ERR_NOTEXTTOSEND("TARGET"));
	RPL_test(ERR_UNKNOWNCOMMAND("TARGET", "COMMAND"));
	RPL_test(ERR_NONICKNAMEGIVEN("TARGET"));
	RPL_test(ERR_NICKNAMEINUSE("TARGET", "NEW_NICK"));
	RPL_test(ERR_NOTONCHANNEL("TARGET", "CHANNEL"));
	RPL_test(ERR_NOTREGISTERED("TARGET"));
	RPL_test(ERR_NEEDMOREPARAMS("TARGET", "COMMAND", "MESSAGE"));
	RPL_test(ERR_ALREADYREGISTERED("TARGET"));
	RPL_test(ERR_PASSWDMISMATCH("TARGET"));
	RPL_test(ERR_CHANNELISFULL("TARGET", "CHANNEL"));
	RPL_test(ERR_INVITEONLYCHAN("TARGET", "CHANNEL"));
	RPL_test(ERR_BANNEDFROMCHAN("TARGET", "CHANNEL"));
	RPL_test(ERR_BADCHANNELKEY("TARGET", "CHANNEL"));
	RPL_test(ERR_CHANOPRIVSNEEDED("TARGET", "CHANNEL"));
	RPL_test(ERR_UMODEUNKNOWNFLAG("TARGET", "FLAG"));
	RPL_test(ERR_USERSDONTMATCH("TARGET", "WHAT"));
	RPL_test(ERR_INVALIDMODEPARAM("TARGET", "RECIPIENT", 'C', "PARAM",
		"MESSAGE"));

	RPL_ADMINME
	RPL_ADMINLOC1
	RPL_ADMINLOC2
	RPL_ADMINEMAIL
	RPL_LISTSTART
	RPL_LIST
	RPL_LISTEND
	RPL_INVITING
	RPL_VERSION
	ERR_USERNOTINCHANNEL
	ERR_USERONCHANNEL
	ERR_NOPRIVILEGES
	ERR_HELPNOTFOUND
	RPL_HELPSTART
	RPL_HELPTXT
	RPL_ENDOFHELP
}

int main()
{
	test_rpl();
}
