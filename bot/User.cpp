#include "User.hpp"

User::User() 
{
}


User::~User() {}

void User::setNickname(const std::string &nick)
{
	_nickname = nick;
}

const std::string User::getNickname() const
{
	return this->_nickname;
}
