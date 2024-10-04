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


// --------------------- //
// ERROR RESPONSES       //
// --------------------- //

#define ERR_NOSUCHNICK(servername, nickname) (std::string(":") + servername + " 401 " + nickname + " :No such nick/channel\r\n")

#define ERR_NOSUCHSERVER(servername, nickname) (std::string(":") + servername + " 402 " + nickname + " :No such server\r\n")

#define ERR_NOSUCHCHANNEL(servername, nickname) (std::string(":") + servername + " 403 " + nickname + " :No such channel\r\n")

#define ERR_NOORIGIN(servername, nickname) (std::string(":") + servername + " 409 " + nickname + " :No origin specified\r\n")

#define ERR_NONICKNAMEGIVEN(servername, nickname) (std::string(":") + servername + " 431 " + nickname + " :No nickname given\r\n")

#define ERR_ERRONEUSNICKNAME(servername, nickname) (std::string(":") + servername + " 432 " + nickname + " :Erroneus nickname\r\n")

#define ERR_NICKNAMEINUSE(servername, nickname) (std::string(":") + servername + " 433 " + nickname + " :Nickname is already in use\r\n")

#define ERR_NEEDMOREPARAMS(servername, nickname, cmd) (std::string(":") + servername + " 461 " + nickname + cmd + " :Not enough parameters\r\n")

#define ERR_ALREADYREGISTRED(servername, nickname) (std::string(":") + servername + " 462 " + nickname + " :You may not reregister\r\n")

#define ERR_PASSWDMISMATCH(servername, nickname) (std::string(":") + servername + " 464 " + nickname + " :Password incorrect\r\n")
