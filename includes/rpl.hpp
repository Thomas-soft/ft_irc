#pragma once

#include "Server.hpp"


// --------------------- //
// REGISTER RESPONSES    //
// --------------------- //

#define RPL_WELCOME(servername, nickname) (std::string(":") + servername + " 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "\r\n")

#define RPL_YOURHOST(servername, nickname) (std::string(":") + servername + " 002 " + nickname + " :Your host is " + servername + ", running version 1.0\r\n")

#define RPL_CREATED(servername, nickname) (std::string(":") + servername + " 003 " + nickname + " :This server was created " + __DATE__ + "\r\n")

#define RPL_MYINFO(servername, nickname, countUser) (std::string(":") + servername + " 004 " + nickname + ": There are " + countUser + " users connected\r\n")

#define RPL_ISUPPORT(servername, nickname) (std::string(":") + servername + " 005 " + nickname + " : The channels must begin with a '#'\r\nNicklen = 30\r\nChanModes = 'i','k','l','o','t'\r\n")


// --------------------- //
// SERVER RESPONSES      //
// --------------------- //

#define RPL_PING(servername) (std::string(":") + servername + " PONG :" + servername + "\r\n")

#define RPL_NOTOPIC(servername, nickname, channel) (std::string(":") + servername + " 331 " + nickname + " " + channel + " :No topic is set\r\n")

#define RPL_INVITING(servername, nickname, channel, allNicknames) (std::string(":") + servername + " 341 " + nickname + " " + allNicknames + " " + channel + "\r\n")

#define RPL_TOPIC(servername, nickname, channel, topic) (std::string(":") + servername + " 332 " + nickname + " " + channel + " :" + topic + "\r\n")

#define RPL_NAMREPLY(servername, nickname, channel, allNicknames) (std::string(":") + servername + " 353 " + nickname + " = " + channel + " :" + allNicknames + "\r\n")

#define RPL_ENDOFNAMES(servername, nickname, channel) (std::string(":") + servername + " 366 " + nickname + " " + channel + " :End of /NAMES list\r\n")


// --------------------- //
// CLIENT NOTIFICATIONS  //
// --------------------- //

#define JOIN_NOTIFY(nickname, username, hostname, channel) (std::string(":") + nickname + "!" + username + "@" + hostname + " JOIN :" + channel + "\r\n")

#define PART_NOTIFY(nickname, username, hostname, channel, reason) (std::string(":") + nickname + "!" + username + "@" + hostname + " PART " + channel + " :" + reason + "\r\n")

#define QUIT_NOTIFY(nickname, username, hostname, reason) (std::string(":") + nickname + "!" + username + "@" + hostname + " QUIT :" + reason + "\r\n")

#define NICK_NOTIFY(oldNickname, newNickname) (std::string(":") + oldNickname + " NICK " + newNickname + "\r\n")

#define MODE_NOTIFY(nickname, username, hostname, channel, mode) (std::string(":") + nickname + "!" + username + "@" + hostname + " MODE " + channel + " " + mode + "\r\n")

#define TOPIC_NOTIFY(nickname, username, hostname, channel, topic) (std::string(":") + nickname + "!" + username + "@" + hostname + " TOPIC " + channel + " :" + topic + "\r\n")

#define KICK_NOTIFY(nickname, username, hostname, channel, target, reason) (std::string(":") + nickname + "!" + username + "@" + hostname + " KICK " + channel + " " + target + " :" + reason + "\r\n")

#define PRIVMSG_NOTIFY(nickname, username, hostname, channel, message) (std::string(":") + nickname + "!" + username + "@" + hostname + " PRIVMSG " + channel + " :" + message + "\r\n")


// --------------------- //
// ERROR RESPONSES       //
// --------------------- //

#define ERR_NOSUCHNICK(servername, nickname) (std::string(":") + servername + " 401 " + nickname + " :No such nick/channel\r\n")

#define ERR_NOSUCHSERVER(servername, nickname) (std::string(":") + servername + " 402 " + nickname + " :No such server\r\n")

#define ERR_NOSUCHCHANNEL(servername, nickname) (std::string(":") + servername + " 403 " + nickname + " :No such channel\r\n")

#define ERR_CANNOTSENDTOCHAN(servername, nickname, channel) (std::string(":") + servername + " 404 " + nickname + " " + channel + " :Cannot send to channel\r\n")

#define ERR_TOOMANYCHANNELS(servername, nickname) (std::string(":") + servername + " 405 " + nickname + " :You have joined too many channels\r\n")

#define ERR_TOOMANYTARGETS(servername, nickname) (std::string(":") + servername + " 407 " + nickname + " :Too many recipients\r\n")


#define ERR_NOORIGIN(servername, nickname) (std::string(":") + servername + " 409 " + nickname + " :No origin specified\r\n")

#define ERR_NORECIPIENT(servername, nickname) (std::string(":") + servername + " 411 " + nickname + " :No recipient given\r\n")

#define ERR_NOTEXTTOSEND(servername, nickname) (std::string(":") + servername + " 412 " + nickname + " :No text to send\r\n")

#define ERR_NONICKNAMEGIVEN(servername, nickname) (std::string(":") + servername + " 431 " + nickname + " :No nickname given\r\n")

#define ERR_ERRONEUSNICKNAME(servername, nickname) (std::string(":") + servername + " 432 " + nickname + " :Erroneus nickname\r\n")

#define ERR_ALREADYONCHANNEL(servername, nickname) (std::string(":") + servername + " 443 " + nickname + " :You're already on that channel\r\n")

#define ERR_NICKNAMEINUSE(servername, nickname) (std::string(":") + servername + " 433 " + nickname + " :Nickname is already in use\r\n")

#define ERR_NOTONCHANNEL(servername, nickname) (std::string(":") + servername + " 442 " + nickname + " :You're not on that channel\r\n")

#define ERR_NEEDMOREPARAMS(servername, nickname, cmd) (std::string(":") + servername + " 461 " + nickname + cmd + " :Not enough parameters\r\n")

#define ERR_ALREADYREGISTRED(servername, nickname) (std::string(":") + servername + " 462 " + nickname + " :You may not reregister\r\n")

#define ERR_PASSWDMISMATCH(servername, nickname) (std::string(":") + servername + " 464 " + nickname + " :Password incorrect\r\n")

#define ERR_CHANNELISFULL(servername, nickname) (std::string(":") + servername + " 471 " + nickname + " :Cannot join channel (+l)\r\n")

#define ERR_INVITEONLYCHAN(servername, nickname) (std::string(":") + servername + " 473 " + nickname + " :Cannot join channel (+i)\r\n")

#define ERR_BADCHANNELKEY(servername, nickname) (std::string(":") + servername + " 475 " + nickname + " :Cannot join channel (+k)\r\n")

#define ERR_BADCHANMASK(servername, nickname) (std::string(":") + servername + " 476 " + nickname + " :Bad Channel Mask\r\n")

#define ERR_USERONCHANNEL(servername, nickname) (std::string(":") + servername + " 477 " + nickname + " :User is already on channel\r\n")

#define ERR_CHANOPRIVSNEEDED(servername, nickname) (std::string(":") + servername + " 482 " + nickname + " :You're not channel operator\r\n")

#define RPL_PART(nickname, username, hostname, channel) (std::string(":") + nickname + "!" + username + "@" + hostname + " PART "  + channel + "\r\n")

