/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCcodes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:45:09 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:46:03 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef unsigned short irc_code_t;

namespace tlucanti::IRCcodes
{
	extern irc_code_t ERR_NOSUCHNICK;
	extern irc_code_t ERR_NOSUCHSERVER;
	extern irc_code_t ERR_NOSUCHCHANNEL;
	extern irc_code_t ERR_CANNOTSENDTOCHAN;
	extern irc_code_t ERR_TOOMANYCHANNELS;
	extern irc_code_t ERR_WASNOSUCHNICK;
	extern irc_code_t ERR_TOOMANYTARGETS;
	extern irc_code_t ERR_NOORIGIN;
	extern irc_code_t ERR_NORECIPIENT;
	extern irc_code_t ERR_NOTEXTTOSEND;
	extern irc_code_t ERR_NOTOPLEVEL;
	extern irc_code_t ERR_WILDTOPLEVEL;
	extern irc_code_t ERR_UNKNOWNCOMMAND;
	extern irc_code_t ERR_NOMOTD;
	extern irc_code_t ERR_NOADMININFO;
	extern irc_code_t ERR_FILEERROR;
	extern irc_code_t ERR_NONICKNAMEGIVEN;
	extern irc_code_t ERR_ERRONEUSNICKNAME;
	extern irc_code_t ERR_NICKNAMEINUSE;
	extern irc_code_t ERR_NICKCOLLISION;
	extern irc_code_t ERR_USERNOTINCHANNEL;
	extern irc_code_t ERR_NOTONCHANNEL;
	extern irc_code_t ERR_USERONCHANNEL;
	extern irc_code_t ERR_NOLOGIN;
	extern irc_code_t ERR_SUMMONDISABLED;
	extern irc_code_t ERR_USERSDISABLED;
	extern irc_code_t ERR_NOTREGISTERED;
	extern irc_code_t ERR_NEEDMOREPARAMS;
	extern irc_code_t ERR_ALREADYREGISTRED;
	extern irc_code_t ERR_NOPERMFORHOST;
	extern irc_code_t ERR_PASSWDMISMATCH;
	extern irc_code_t ERR_YOUREBANNEDCREEP;
	extern irc_code_t ERR_KEYSET;
	extern irc_code_t ERR_CHANNELISFULL;
	extern irc_code_t ERR_UNKNOWNMODE;
	extern irc_code_t ERR_INVITEONLYCHAN;
	extern irc_code_t ERR_BANNEDFROMCHAN;
	extern irc_code_t ERR_BADCHANNELKEY;
	extern irc_code_t ERR_NOPRIVILEGES;
	extern irc_code_t ERR_CHANOPRIVSNEEDED;
	extern irc_code_t ERR_CANTKILLSERVER;
	extern irc_code_t ERR_NOOPERHOST;
	extern irc_code_t ERR_UMODEUNKNOWNFLAG;
	extern irc_code_t ERR_USERSDONTMATCH;
	extern irc_code_t RPL_NONE;
	extern irc_code_t RPL_USERHOST;
	extern irc_code_t RPL_ISON;
	extern irc_code_t RPL_AWAY;
	extern irc_code_t RPL_UNAWAY;
	extern irc_code_t RPL_NOWAWAY;
	extern irc_code_t RPL_WHOISUSER;
	extern irc_code_t RPL_WHOISSERVER;
	extern irc_code_t RPL_WHOISOPERATOR;
	extern irc_code_t RPL_WHOISIDLE;
	extern irc_code_t RPL_ENDOFWHOIS;
	extern irc_code_t RPL_WHOISCHANNELS;
	extern irc_code_t RPL_WHOWASUSER;
	extern irc_code_t RPL_ENDOFWHOWAS;
	extern irc_code_t RPL_LISTSTART;
	extern irc_code_t RPL_LIST;
	extern irc_code_t RPL_LISTEND;
	extern irc_code_t RPL_CHANNELMODEIS;
	extern irc_code_t RPL_NOTOPIC;
	extern irc_code_t RPL_TOPIC;
	extern irc_code_t RPL_INVITING;
	extern irc_code_t RPL_SUMMONING;
	extern irc_code_t RPL_VERSION;
	extern irc_code_t RPL_WHOREPLY;
	extern irc_code_t RPL_ENDOFWHO;
	extern irc_code_t RPL_NAMREPLY;
	extern irc_code_t RPL_ENDOFNAMES;
	extern irc_code_t RPL_LINKS;
	extern irc_code_t RPL_ENDOFLINKS;
	extern irc_code_t RPL_BANLIST;
	extern irc_code_t RPL_ENDOFBANLIST;
	extern irc_code_t RPL_INFO;
	extern irc_code_t RPL_ENDOFINFO;
	extern irc_code_t RPL_MOTDSTART;
	extern irc_code_t RPL_MOTD;
	extern irc_code_t RPL_ENDOFMOTD;
	extern irc_code_t RPL_YOUREOPER;
	extern irc_code_t RPL_REHASHING;
	extern irc_code_t RPL_TIME;
	extern irc_code_t RPL_USERSSTART;
	extern irc_code_t RPL_USERS;
	extern irc_code_t RPL_ENDOFUSERS;
	extern irc_code_t RPL_NOUSERS;
	extern irc_code_t RPL_TRACELINK;
	extern irc_code_t RPL_TRACECONNECTING;
	extern irc_code_t RPL_TRACEHANDSHAKE;
	extern irc_code_t RPL_TRACEUNKNOWN;
	extern irc_code_t RPL_TRACEOPERATOR;
	extern irc_code_t RPL_TRACEUSER;
	extern irc_code_t RPL_TRACESERVER;
	extern irc_code_t RPL_TRACENEWTYPE;
	extern irc_code_t RPL_TRACELOG;
	extern irc_code_t RPL_STATSLINKINFO;
	extern irc_code_t RPL_STATSCOMMANDS;
	extern irc_code_t RPL_STATSCLINE;
	extern irc_code_t RPL_STATSNLINE;
	extern irc_code_t RPL_STATSILINE;
	extern irc_code_t RPL_STATSKLINE;
	extern irc_code_t RPL_STATSYLINE;
	extern irc_code_t RPL_ENDOFSTATS;
	extern irc_code_t RPL_UMODEIS;
	extern irc_code_t RPL_STATSLLINE;
	extern irc_code_t RPL_STATSUPTIME;
	extern irc_code_t RPL_STATSOLINE;
	extern irc_code_t RPL_STATSHLINE;
	extern irc_code_t RPL_LUSERCLIENT;
	extern irc_code_t RPL_LUSEROP;
	extern irc_code_t RPL_LUSERUNKNOWN;
	extern irc_code_t RPL_LUSERCHANNELS;
	extern irc_code_t RPL_LUSERME;
	extern irc_code_t RPL_ADMINME;
	extern irc_code_t RPL_ADMINLOC1;
	extern irc_code_t RPL_ADMINLOC2;
	extern irc_code_t RPL_ADMINEMAIL;
}
