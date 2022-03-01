#include "Message.hpp"

Message::Message() {}

Message::Message(const Message &rhs) : _prefix(rhs._prefix),
                                       _parameters(rhs._parameters),
                                       _command(rhs._command),
                                       _isPrefix(rhs._isPrefix),
                                       _isCommand(rhs._isCommand)
{
}

Message::~Message() {}

Message::Message(std::string str, const User &usr)
    : _isPrefix(false), _isCommand(false)
{
    this->_parse(str, usr);
    std::stringstream ss(_prefix);
    std::getline(ss, _name, '!');
    std::cout << "prefix: <" << _prefix << "> _name: <"<< _name << ">\n";
}

void Message::setCommand(const std::string &command)
{
    _command = command;
}

const std::string &Message::getCommand() const { return this->_command; }

const std::vector<std::string> &Message::getParamets() const { return this->_parameters; }

const std::string &Message::getPrefix() const { return this->_prefix; }

const std::string &Message::getName() const { return _name; }

void Message::_parse(std::string str, const User &usr)
{
    std::vector<std::string> vec_sep_space;
    std::vector<std::string> vec_sep_s2;

    this->_prefix = usr.getNickname();

    if (str[0] == ':')
        this->_isPrefix = true;

    if (_check_s1(str) == 1)
    {
        std::string buf_str;

        vec_sep_s2 = _split(str, ':');
        for (size_t i = 1; i < vec_sep_s2.size(); ++i)
            buf_str += vec_sep_s2[i];
        vec_sep_space = _split(vec_sep_s2[0], ' ');
        _parseUtility(vec_sep_space);
        this->_parameters.push_back(buf_str);
    }
    else
    {
        vec_sep_space = _split(str, ' ');
        _parseUtility(vec_sep_space);
    }
}

std::vector<std::string> Message::_split(const std::string &str, char delimeter)
{
    std::vector<std::string> result;
    std::istringstream sstream(str);
    std::string tmp;

    while (std::getline(sstream, tmp, delimeter))
    {
        if (!tmp.empty())
            result.push_back(tmp);
    }
    return result;
}

int Message::_check_s1(const std::string &str)
{
    for (size_t i = 0; i != str.size(); ++i)
    {
        if (str[i] == ':')
            return 1;
    }
    return 0;
}

int Message::_check_s2(const std::string &str)
{
    for (size_t i = 0; i != str.size(); ++i)
    {
        if (str[i] == ',')
            return 1;
    }
    return 0;
}

void Message::_parseUtility(std::vector<std::string> vec_sep_space)
{
    std::vector<std::string> vec_sep_comma;

    if (!vec_sep_space.empty())
    {
        for (size_t i = 0; i != vec_sep_space.size(); ++i)
        {
            if (_isPrefix && i == 0)
            {
                this->_prefix = vec_sep_space[i];
            }
            else if (!_isCommand)
            {
                this->_command = vec_sep_space[i];
                _isCommand = true;
            }
            else
            {
                if (_check_s2(vec_sep_space[i]) == 1)
                {
                    vec_sep_comma = _split(vec_sep_space[i], ',');
                    for (size_t j = 0; j < vec_sep_comma.size(); ++j)
                        this->_parameters.push_back(vec_sep_comma[j]);
                }
                else
                    this->_parameters.push_back(vec_sep_space[i]);
            }
        }
    }
}
