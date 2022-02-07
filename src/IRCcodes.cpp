/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCcodes.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:42:50 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/07 21:55:16 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef unsigned short irc_code_t;

namespace tlucanti::IRCcodes
{
	irc_code_t RPL_WELCOME			= 001;
	irc_code_t RPL_YOURHOST			= 002;
	irc_code_t RPL_CREATED			= 003;
	irc_code_t RPL_MYINFO			= 004;
	irc_code_t RPL_ISUPPORT			= 005;
	irc_code_t RPL_BOUNCE			= 010;
	irc_code_t RPL_UMODEIS			= 221;
	irc_code_t RPL_LUSERCLIENT		= 251;
	irc_code_t RPL_LUSEROP			= 252;
	irc_code_t RPL_LUSERUNKNOWN		= 253;
	irc_code_t RPL_LUSERCHANNELS	= 254;
	irc_code_t RPL_LUSERME			= 255;
	irc_code_t RPL_ADMINME			= 256;
	irc_code_t RPL_ADMINLOC1		= 257;
	irc_code_t RPL_ADMINLOC2		= 258;
	irc_code_t RPL_ADMINEMAIL		= 259;
	irc_code_t RPL_TRYAGAIN			= 263;
	irc_code_t RPL_LOCALUSERS		= 265;
	irc_code_t RPL_GLOBALUSERS		= 266;
	irc_code_t RPL_WHOISCERTFP		= 276;
	irc_code_t RPL_NONE				= 300;
	irc_code_t RPL_AWAY				= 301;
	irc_code_t RPL_USERHOST			= 302;
	irc_code_t RPL_UNAWAY			= 305;
	irc_code_t RPL_NOWAWAY			= 306;
	irc_code_t RPL_WHOREPLY			= 352;
	irc_code_t RPL_ENDOFWHO			= 315;
	irc_code_t RPL_WHOISREGNICK		= 307;
	irc_code_t RPL_WHOISUSER		= 311;
	irc_code_t RPL_WHOISSERVER		= 312;
	irc_code_t RPL_WHOISOPERATOR	= 313;
	irc_code_t RPL_WHOWASUSER		= 314;
	irc_code_t RPL_WHOISIDLE		= 317;
	irc_code_t RPL_ENDOFWHOIS		= 318;
	irc_code_t RPL_WHOISCHANNELS	= 319;
	irc_code_t RPL_WHOISSPECIAL		= 320;
	irc_code_t RPL_LISTSTART		= 321;
	irc_code_t RPL_LIST				= 322;
	irc_code_t RPL_LISTEND			= 323;
	irc_code_t RPL_CHANNELMODEIS	= 324;
	irc_code_t RPL_CREATIONTIME		= 329;
	irc_code_t RPL_WHOISACCOUNT		= 330;
	irc_code_t RPL_NOTOPIC			= 331;
	irc_code_t RPL_TOPIC			= 332;
	irc_code_t RPL_TOPICWHOTIME		= 333;
	irc_code_t RPL_WHOISACTUALLY	= 338;
	irc_code_t RPL_INVITING			= 341;
	irc_code_t RPL_INVITELIST		= 346;
	irc_code_t RPL_ENDOFINVITELIST	= 347;
	irc_code_t RPL_EXCEPTLIST		= 348;
	irc_code_t RPL_ENDOFEXCEPTLIST	= 349;
	irc_code_t RPL_VERSION			= 351;
	irc_code_t RPL_NAMREPLY			= 353;
	irc_code_t RPL_ENDOFNAMES		= 366;
	irc_code_t RPL_BANLIST			= 367;
	irc_code_t RPL_ENDOFBANLIST		= 368;
	irc_code_t RPL_ENDOFWHOWAS		= 369;
	irc_code_t RPL_INFO				= 371;
	irc_code_t RPL_ENDOFINFO		= 374;
	irc_code_t RPL_MOTDSTART		= 375;
	irc_code_t RPL_MOTD				= 372;
	irc_code_t RPL_ENDOFMOTD		= 376;
	irc_code_t RPL_WHOISHOST		= 378;
	irc_code_t RPL_WHOISMODES		= 379;
	irc_code_t RPL_YOUREOPER		= 381;
	irc_code_t RPL_REHASHING		= 382;
	irc_code_t RPL_TIME				= 391;
	irc_code_t ERR_UNKNOWNERROR		= 400;
	irc_code_t ERR_NOSUCHNICK		= 401;
	irc_code_t ERR_NOSUCHSERVER		= 402;
	irc_code_t ERR_NOSUCHCHANNEL	= 403;
	irc_code_t ERR_CANNOTSENDTOCHAN	= 404;
	irc_code_t ERR_TOOMANYCHANNELS	= 405;
	irc_code_t ERR_NOORIGIN			= 409;
	irc_code_t ERR_INPUTTOOLONG		= 417;
	irc_code_t ERR_UNKNOWNCOMMAND	= 421;
	irc_code_t ERR_NOMOTD			= 422;
	irc_code_t ERR_ERRONEUSNICKNAME	= 432;
	irc_code_t ERR_NICKNAMEINUSE	= 433;
	irc_code_t ERR_USERNOTINCHANNEL	= 441;
	irc_code_t ERR_NOTONCHANNEL		= 442;
	irc_code_t ERR_USERONCHANNEL	= 443;
	irc_code_t ERR_NOTREGISTERED	= 451;
	irc_code_t ERR_NEEDMOREPARAMS	= 461;
	irc_code_t ERR_ALREADYREGISTERED= 462;
	irc_code_t ERR_PASSWDMISMATCH	= 464;
	irc_code_t ERR_YOUREBANNEDCREEP	= 465;
	irc_code_t ERR_CHANNELISFULL	= 471;
	irc_code_t ERR_UNKNOWNMODE		= 472;
	irc_code_t ERR_INVITEONLYCHAN	= 473;
	irc_code_t ERR_BANNEDFROMCHAN	= 474;
	irc_code_t ERR_BADCHANNELKEY	= 475;
	irc_code_t ERR_BADCHANMASK		= 476;
	irc_code_t ERR_NOPRIVILEGES		= 481;
	irc_code_t ERR_CHANOPRIVSNEEDED	= 482;
	irc_code_t ERR_CANTKILLSERVER	= 483;
	irc_code_t ERR_NOOPERHOST		= 491;
	irc_code_t ERR_UMODEUNKNOWNFLAG	= 501;
	irc_code_t ERR_USERSDONTMATCH	= 502;
	irc_code_t ERR_HELPNOTFOUND		= 524;
	irc_code_t ERR_INVALIDKEY		= 525;
	irc_code_t RPL_STARTTLS			= 670;
	irc_code_t RPL_WHOISSECURE		= 671;
	irc_code_t ERR_STARTTLS			= 691;
	irc_code_t ERR_INVALIDMODEPARAM	= 696;
	irc_code_t RPL_HELPSTART		= 704;
	irc_code_t RPL_HELPTXT			= 705;
	irc_code_t RPL_ENDOFHELP		= 706;
	irc_code_t ERR_NOPRIVS			= 723;
	irc_code_t RPL_LOGGEDIN			= 900;
	irc_code_t RPL_LOGGEDOUT		= 901;
	irc_code_t ERR_NICKLOCKED		= 902;
	irc_code_t RPL_SASLSUCCESS		= 903;
	irc_code_t ERR_SASLFAIL			= 904;
	irc_code_t ERR_SASLTOOLONG		= 905;
	irc_code_t ERR_SASLABORTED		= 906;
	irc_code_t ERR_SASLALREADY		= 907;
	irc_code_t RPL_SASLMECHS		= 908;
}
