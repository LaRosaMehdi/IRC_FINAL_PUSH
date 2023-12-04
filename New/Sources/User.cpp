#include "./../Includes/User.hpp"

// Constructor

int defaultUserIndex = 0; 

User::User(const int socket, const struct sockaddr_in& addr)
    : _fd(socket), _address(addr)
{

    setUsername("default" + std::to_string(defaultUserIndex));
    setNickname("default" + std::to_string(defaultUserIndex));
    setRealname("default" + std::to_string(defaultUserIndex));
    setHostname("default" + std::to_string(defaultUserIndex));
    setAccepted(false);
    setConnected(false);
    defaultUserIndex += 1;

    // For commands AKA Do
    _checkpwd = false;
    _checknick = false;
    _checkuser = false;
}

User::User(const std::string& username, const std::string& nickname, int sock, const struct sockaddr_in& addr)
    : _username(username), _nickname(nickname), _fd(sock), _address(addr)
{
    setAccepted(false);
    setConnected(false);
}

User::User(const std::string& username, const std::string& nickname, int sock, const struct sockaddr_in& addr, bool accepted, bool connected)
: _username(username), _nickname(nickname), _fd(sock), _address(addr), _accepted(accepted), _connected(connected){
}

// Destructor

User::~User() {}

// Getters

std::string User::getUsername() const { return _username; }
std::string User::getNickname() const { return _nickname; }
std::string User::getRealname() const { return _realname; }
std::string User::getHostname() const { return _hostname; }
struct sockaddr_in User::getAddress() const { return _address; }
int User::getSocket() const { return _fd; }
std::string User::getCompleteName() const { return _username + "!" + _nickname + "@" + _hostname; }


// Setters

void User::setUsername(const std::string& username) { _username = username; }
void User::setNickname(const std::string& nickname) { _nickname = nickname; }
void User::setRealname(const std::string& realname) { _realname = realname; }
void User::setHostname(const std::string& hostname) { _hostname = hostname; }
void User::setAddress(const struct sockaddr_in& address) { _address = address; }
void User::setSocket(int socket) { _fd = socket; }
void User::setAccepted(bool accepted) { _accepted = accepted; }
void User::setConnected(bool connected) { _connected = connected; }

// Is

bool User::isAccepted() const { return _accepted; }
bool User::isConnected() const { return _connected; }

// For commands AKA Do

bool User::isCheckNick() const { return _checknick; }
bool User::isCheckPwd() const { return _checkpwd; }
bool User::isCheckUser() const { return _checkuser; }

// Switch its states

void User::switchNick()  {_checknick = true; }
void User::switchPwd()  {  _checkpwd = true; }
void User::switchUser()  {  _checkuser = true; }

// Send Message 
bool    User::sendRawMessage(std::string message) {
    return send(this->getSocket(), message.c_str(), message.length(), 0) != -1;
}

bool User::formatSendMessage(std::string code, std::string message)
{
    std::stringstream ss;

    ss	<< ":ft_irc "
        << code << " "
        << this->getNickname() << " "
        << message << "\r\n";

    return this->sendRawMessage(ss.str());
}
