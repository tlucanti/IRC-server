#ifndef USER_HPP
#define USER_HPP

#include <iostream>

class User {

	public:
		User();
		~User();

	private:
		std::string _nickname;
	public:
		void	setNickname(const std::string &nick);
		const std::string getNickname() const;

};

#endif
