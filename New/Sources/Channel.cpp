#include "./../Includes/Channel.hpp"

// Constructor / Destructor

Channel::Channel(const std::string& name, User *op) : _name(name), _inviteOnly(false), _topicRestriction(false), _keyRestriction(false), _password(""){
    setTopic("");
    setOperator(op);
    _users.push_back(op);
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

void    Channel::joinOperator(User* user)
{
    _operator.push_back(user);
    // sendBroadcastMessage(":" + user->getCompleteName() + " JOIN #" + getName() + "\r\n", user);
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

User    *Channel::getFirstOperator() const {return *_operator.begin() ;}

std::vector<User *>Channel::getOperator() const { return _operator; }

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

User* Channel::getOperatorByUsername(const std::string& username) {
    for (int i = 0; i < (int)getOperator().size(); i++) {
        if (getOperator()[i]->getUsername() == username) 
            return getOperator()[i];         
    }
    return NULL;
}


User* Channel::getUserByUsername(const std::string& username) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getUsername() == username) 
            return getUsers()[i];         
    }
    return NULL;
}



// Setters
void		Channel::clearTopic(void) { _topic.clear();}
void Channel::setName(const std::string &name) { _name = name; }

void Channel::setPassword(const std::string &pass) { _password = pass; }

void Channel::setTopic(const std::string &topic) { _topic = topic; }

void Channel::setOperator(User *op) { _operator.push_back(op); }

// Is

bool Channel::isOperator(User *user) const { 
    if (!_operator.empty()) {
        std::vector<User*>::const_iterator it = _operator.begin();
        for (; it != _operator.end(); it++){
            if (user == *it)
                return true;
            }
    }
    return false;
}


//Mode 

bool Channel::getInviteBool() const { return _inviteOnly; }

void    Channel::setChannelPass(std::string pass){ std::cout << pass << std::endl;
    std::cout << "oue " << std::endl;
    _password = pass.substr(0, pass.size());
    std::cout << "_password: " << _password << std::endl;
    _keyRestriction = true;
}

void    Channel::removeChannelPass(void){
    _password = "";
    _keyRestriction = false;
}

std::string    Channel::getPassword(void){ return  _password; }

bool    Channel::getPasswordRestriction(void){ return  _keyRestriction; }

bool    Channel::getTopicRestriction(void){ return  _topicRestriction; }


void    Channel::setTopicRestriction(bool pass){ std::cout << pass << std::endl;
    _topicRestriction = pass; }

void    Channel::setInviteOnly(bool invit){ std::cout << invit << std::endl;
    _inviteOnly = invit; }

bool    Channel::isInvitedByOperator(User *invit){
    if (!_invited.empty()) {
        std::vector<User*>::const_iterator it = _invited.begin();
        for (; it != _operator.end(); it++){
            if (invit == *it)
                return true;
            }
    }
    return false;
}

bool    Channel::addInvited(User *ope, User *invit){
    if (ope && invit){
        _invited.push_back(invit);
        return true;
    }
    return false;
}

void    Channel::giveTakeOperator(User* user, bool bol){
    if (bol == true){
        if (getOperatorByUsername(user->getUsername())){
            std::cout << "Error: UserTarget is already an operator!" << std::endl;
        }
        else if (!getOperatorByUsername(user->getUsername())){
            setOperator(user);
            std::cout << user->getUsername() << "became a new operator of " << getName() << std::endl;
        }
    }
    else if (bol == false){
        if (!getOperatorByUsername(user->getUsername())){
            std::cout << "Error: UserTarget is not an operator!" << std::endl;
        }
        else{
            std::vector<User *>::iterator it = std::find(getOperator().begin(), getOperator().end(), user);
            if (it != getOperator().end()) {
                getOperator().erase(it);
                std::cout << user->getUsername() << "lose operator right." << std::endl;
                for (std::vector<User *>::iterator iter = getOperator().begin(); iter != getOperator().end(); ++iter) {
                    std::cout << *iter << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}