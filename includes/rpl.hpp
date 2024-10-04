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
// ERROR RESPONSES       //
// --------------------- //

#define ERR_NOSUCHNICK(servername, nickname) ":" #servername " 401 " #nickname " :No such nick/channel\r\n"

#define ERR_NOSUCHCHANNEL(servername, nickname, channel) ":" #servername " 403 " #nickname " " #channel " :No such channel\r\n"

#define ERR_NONICKNAMEGIVEN(servername, nickname) ":" #servername " 431 " #nickname " :No nickname given\r\n"

#define ERR_ERRONEUSNICKNAME(servername, nickname) ":" #servername " 432 " #nickname " :Erroneus nickname\r\n"

#define ERR_NICKNAMEINUSE(servername, nickname) ":" #servername " 433 " #nickname " :Nickname is already in use\r\n"

#define ERR_NEEDMOREPARAMS(servername, nickname, cmd) ":" #servername " 461 " #nickname #cmd " :Not enough parameters\r\n"

#define ERR_ALREADYREGISTRED(servername, nickname) ":" #servername " 462 " #nickname " :You may not reregister\r\n"

#define ERR_PASSWDMISMATCH(servername, nickname) ":" #servername " 464 " #nickname " :Password incorrect\r\n"
