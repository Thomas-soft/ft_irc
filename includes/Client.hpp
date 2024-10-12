#pragma once

#include "Server.hpp"

class Client
{
	private:
		int _fd;
		std::string _password;
		std::string _nickname;
		std::string _username;
		std::string _buffer;
		std::string _hostname;
		std::string _servername;
		std::string _realname;
		bool        _registered;

	public:
		Client(int fd, char *ip);

		// --------------------- //
		// Client getters        //
		// --------------------- //
		int	get_fd() const;
		std::string get_password() const;
		std::string get_nickname() const;
		std::string get_username() const;
		std::string get_hostname() const;
		std::string get_buffer() const;
		std::string get_servername() const;
		std::string get_realname() const;

		// --------------------- //
		// Client setters        //
		// --------------------- //
		bool        is_registered() const;
		void		set_password(std::string password);
		void 		set_buffer(std::string buffer);
		void		set_nickname(std::string nickname);
		void		set_username(std::string username);
		void		set_hostname(std::string hostname);
		void		set_servername(std::string servername);
		void		set_realname(std::string realname);
		void        set_registered(bool registered);
		~Client();
};
