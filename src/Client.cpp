#include "../includes/Client.hpp"
/*
Client::Client(int fd, std::string nick, std::string user, std::string pass): _fd(fd), _nick(nick), _user(user), _pass(pass)
{

}*/

Client::Client(int fd) : _fd(fd)
{
	std::memset(this, 0, sizeof(Client));
	_password = "";
	_nickname = "";
	_username = "";
	_hostname = "";
	_servername = "";
	_realname = "";
	_registered = false;
}


Client::~Client()
{

}

int Client::get_fd() const
{
	return (_fd);
}

std::string Client::get_password() const
{
	return (_password);
}

std::string Client::get_nickname() const
{
	return (_nickname);
}

std::string Client::get_username() const
{
	return (_username);
}

std::string Client::get_hostname() const
{
	return (_hostname);
}

std::string Client::get_servername() const
{
	return (_servername);
}

std::string Client::get_realname() const
{
	return (_realname);
}

bool Client::is_registered() const
{
	return (_registered);
}

void Client::set_password(std::string password)
{
	_password = password;
}

void Client::set_nickname(std::string nickname)
{
	_nickname = nickname;
}

void Client::set_username(std::string username)
{
	_username = username;
}

void Client::set_hostname(std::string hostname)
{
	_hostname = hostname;
}

void Client::set_servername(std::string servername)
{
	_servername = servername;
}

void Client::set_realname(std::string realname)
{
	_realname = realname;
}

void Client::set_registered(bool registered)
{
	_registered = registered;
}
