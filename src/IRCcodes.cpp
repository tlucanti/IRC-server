/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCcodes.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:42:50 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:39 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef unsigned short irc_code_t;

namespace tlucanti::IRCcodes
{
	irc_code_t ERR_NOSUCHNICK		= 401;
	irc_code_t ERR_NOSUCHSERVER		= 402;
	irc_code_t ERR_NOSUCHCHANNEL	= 403;
	irc_code_t ERR_CANNOTSENDTOCHAN	= 404;
	irc_code_t ERR_TOOMANYCHANNELS	= 405;
	irc_code_t ERR_WASNOSUCHNICK	= 406;
	irc_code_t ERR_TOOMANYTARGETS	= 407;
	irc_code_t ERR_NOORIGIN			= 409;
	irc_code_t ERR_NORECIPIENT		= 411;
	irc_code_t ERR_NOTEXTTOSEND		= 412;
	irc_code_t ERR_NOTOPLEVEL		= 413;
	irc_code_t ERR_WILDTOPLEVEL		= 414;
	irc_code_t ERR_UNKNOWNCOMMAND	= 421;
	irc_code_t ERR_NOMOTD			= 422;
	irc_code_t ERR_NOADMININFO		= 423;
	irc_code_t ERR_FILEERROR		= 424;
	irc_code_t ERR_NONICKNAMEGIVEN	= 431;
	irc_code_t ERR_ERRONEUSNICKNAME	= 432;
	irc_code_t ERR_NICKNAMEINUSE	= 433;
	irc_code_t ERR_NICKCOLLISION	= 436;
	irc_code_t ERR_USERNOTINCHANNEL	= 441;
	irc_code_t ERR_NOTONCHANNEL		= 442;
	irc_code_t ERR_USERONCHANNEL	= 443;
	irc_code_t ERR_NOLOGIN			= 444;
	irc_code_t ERR_SUMMONDISABLED	= 445;
	irc_code_t ERR_USERSDISABLED	= 446;
	irc_code_t ERR_NOTREGISTERED	= 451;
	irc_code_t ERR_NEEDMOREPARAMS	= 461;
	irc_code_t ERR_ALREADYREGISTRED	= 462;
	irc_code_t ERR_NOPERMFORHOST	= 463;
	irc_code_t ERR_PASSWDMISMATCH	= 464;
	irc_code_t ERR_YOUREBANNEDCREEP	= 465;
	irc_code_t ERR_KEYSET			= 467;
	irc_code_t ERR_CHANNELISFULL	= 471;
	irc_code_t ERR_UNKNOWNMODE		= 472;
	irc_code_t ERR_INVITEONLYCHAN	= 473;
	irc_code_t ERR_BANNEDFROMCHAN	= 474;
	irc_code_t ERR_BADCHANNELKEY	= 475;
	irc_code_t ERR_NOPRIVILEGES		= 481;
	irc_code_t ERR_CHANOPRIVSNEEDED	= 482;
	irc_code_t ERR_CANTKILLSERVER	= 483;
	irc_code_t ERR_NOOPERHOST		= 491;
	irc_code_t ERR_UMODEUNKNOWNFLAG	= 501;
	irc_code_t ERR_USERSDONTMATCH	= 502;

	irc_code_t RPL_NONE				= 300;
	irc_code_t RPL_USERHOST			= 302;
	irc_code_t RPL_ISON				= 303;
	irc_code_t RPL_AWAY				= 301;
	irc_code_t RPL_UNAWAY			= 305;
	irc_code_t RPL_NOWAWAY			= 306;
	irc_code_t RPL_WHOISUSER		= 311;
	irc_code_t RPL_WHOISSERVER		= 312;
	irc_code_t RPL_WHOISOPERATOR	= 313;
	irc_code_t RPL_WHOISIDLE		= 317;
	irc_code_t RPL_ENDOFWHOIS		= 318;
	irc_code_t RPL_WHOISCHANNELS	= 319;
	irc_code_t RPL_WHOWASUSER		= 314;
	irc_code_t RPL_ENDOFWHOWAS		= 369;
	irc_code_t RPL_LISTSTART		= 321;
	irc_code_t RPL_LIST				= 322;
	irc_code_t RPL_LISTEND			= 323;
	irc_code_t RPL_CHANNELMODEIS	= 324;
	irc_code_t RPL_NOTOPIC			= 331;
	irc_code_t RPL_TOPIC			= 332;
	irc_code_t RPL_INVITING			= 341;
	irc_code_t RPL_SUMMONING		= 342;
	irc_code_t RPL_VERSION			= 351;
	irc_code_t RPL_WHOREPLY			= 352;
	irc_code_t RPL_ENDOFWHO			= 315;
	irc_code_t RPL_NAMREPLY			= 353;
	irc_code_t RPL_ENDOFNAMES		= 366;
	irc_code_t RPL_LINKS			= 364;
	irc_code_t RPL_ENDOFLINKS		= 365;
	irc_code_t RPL_BANLIST			= 367;
	irc_code_t RPL_ENDOFBANLIST		= 368;
	irc_code_t RPL_INFO				= 371;
	irc_code_t RPL_ENDOFINFO		= 374;
	irc_code_t RPL_MOTDSTART		= 375;
	irc_code_t RPL_MOTD				= 372;
	irc_code_t RPL_ENDOFMOTD		= 376;
	irc_code_t RPL_YOUREOPER		= 381;
	irc_code_t RPL_REHASHING		= 382;
	irc_code_t RPL_TIME				= 391;
	irc_code_t RPL_USERSSTART		= 392;
	irc_code_t RPL_USERS			= 393;
	irc_code_t RPL_ENDOFUSERS		= 394;
	irc_code_t RPL_NOUSERS			= 395;
	irc_code_t RPL_TRACELINK		= 200;
	irc_code_t RPL_TRACECONNECTING	= 201;
	irc_code_t RPL_TRACEHANDSHAKE	= 202;
	irc_code_t RPL_TRACEUNKNOWN		= 203;
	irc_code_t RPL_TRACEOPERATOR	= 204;
	irc_code_t RPL_TRACEUSER		= 205;
	irc_code_t RPL_TRACESERVER		= 206;
	irc_code_t RPL_TRACENEWTYPE		= 208;
	irc_code_t RPL_TRACELOG			= 261;
	irc_code_t RPL_STATSLINKINFO	= 211;
	irc_code_t RPL_STATSCOMMANDS	= 212;
	irc_code_t RPL_STATSCLINE		= 213;
	irc_code_t RPL_STATSNLINE		= 214;
	irc_code_t RPL_STATSILINE		= 215;
	irc_code_t RPL_STATSKLINE		= 216;
	irc_code_t RPL_STATSYLINE		= 218;
	irc_code_t RPL_ENDOFSTATS		= 219;
	irc_code_t RPL_UMODEIS			= 221;
	irc_code_t RPL_STATSLLINE		= 241;
	irc_code_t RPL_STATSUPTIME		= 242;
	irc_code_t RPL_STATSOLINE		= 243;
	irc_code_t RPL_STATSHLINE		= 244;
	irc_code_t RPL_LUSERCLIENT		= 251;
	irc_code_t RPL_LUSEROP			= 252;
	irc_code_t RPL_LUSERUNKNOWN		= 253;
	irc_code_t RPL_LUSERCHANNELS	= 254;
	irc_code_t RPL_LUSERME			= 255;
	irc_code_t RPL_ADMINME			= 256;
	irc_code_t RPL_ADMINLOC1		= 257;
	irc_code_t RPL_ADMINLOC2		= 258;
	irc_code_t RPL_ADMINEMAIL		= 259;
}
