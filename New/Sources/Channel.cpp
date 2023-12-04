#include "./../Includes/Channel.hpp"

// Constructor / Destructor

Channel::Channel(const std::string& name, User *op) : _name(name), _operator(op) {
    setTopic("");
}

Channel::~Channel() {}


// User management

bool    Channel::removeUser(User* user) {
    std::vector<User*>::iterator it = std::find(_users.begin(), _users.end(), user);
    if (it != _users.end()) {
        _users.erase(it);
        return true;
    }
    return false;
}

void    Channel::joinUser(User* user)
{
    _users.push_back(user);
    sendBroadcastMessage(":" + user->getCompleteName() + " JOIN #" + getName() + "\r\n", user);
}

// Message management

void    Channel::sendBroadcastMessage(const std::string& message, User* sender)
{
    // std::cout << "sendBroadcastMessageing message: " << message << std::endl;
    std::vector<User*>::iterator it = _users.begin();
    while (it != _users.end()) {
        if (*it != sender) {
            send((*it)->getSocket(), message.c_str(), message.size(), 0);
        }
        it++;
    }
}

bool    Channel::sendChannelMessage(std::string code, std::string message, User *user, bool userList) {
    std::string formattedMessage = formatSendChannelMessage(code, message, user, userList);
    // std::cout << "Sending message: |" << formattedMessage << "|" << std::endl;
    send(user->getSocket(), formattedMessage.c_str(), formattedMessage.size(), 0);
    return true;
}

std::string Channel::formatSendChannelMessage(std::string code, std::string message, User *user, bool userList)
{
    std::string send;
    send = ":ft_irc " + code + " " + user->getNickname();
    if (userList == true)
        send += " = #";
    else
        send += " #";
    send += getName() + " :" + message + "\r\n";
    return send;
}

// Getters 
std::string Channel::getName() const { return _name; }

std::vector<User *> Channel::getUsers() const { return _users; }

std::string Channel::getTopic() const { return _topic; }

User *Channel::getOperator() const { return _operator; }

std::string Channel::getUsersList() { 
    std::string usersList;
    for (size_t i = 0; i < _users.size(); i++) {
        if (isOperator(_users[i]))
            usersList += "@";
        usersList += _users[i]->getNickname();
        if (i + 1 < _users.size())
            usersList += " ";
    }
    return usersList;
}

User* Channel::getUserByUsername(const std::string& username) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getUsername() == username) 
            return getUsers()[i];         
    }
    return NULL;
}

// Setters

void Channel::setName(const std::string &name) { _name = name; }

void Channel::setTopic(const std::string &topic) { _topic = topic; }

void Channel::setOperator(User *op) { _operator = op; }

// Is

bool Channel::isOperator(User *user) const { return user == _operator; }
