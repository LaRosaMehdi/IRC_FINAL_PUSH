/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/12/20 16:10:54 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../Includes/Server.hpp"

// Constructor and destructor

Server::Server(int port, const std::string& password)
{
    setPort(port);
    setPassword(password);
    init();
}

Server::~Server() {
    for (int i = getUsers().size() - 1; i >= 0; --i) {
        handleClientDisconnection(getUsers()[i]);
    }
    close(_socket);
    delete _clients;
    logs("LOG", "Server destroyed");
}

// Inits

void Server::init(void)
{
    initSocket();
    initAddress();
    initReadFds();

    int reuse = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        throw std::runtime_error(std::string(ERROR) + "[ERROR]:\t    " + RESET + "setsockopt(SO_REUSEADDR) failed\n");
    }
    if (bind(_socket, (struct sockaddr*)&_address, sizeof(_address)) == -1) {
        throw std::runtime_error( std::string(ERROR) + "[ERROR]:\t    " + RESET + "Binding failed\n");
    }
    if (listen(_socket, 10) == -1) {
        throw std::runtime_error(std::string(ERROR) + "[ERROR]:\t    " + RESET + "Listening failed\n");
    }
    // char buffer[20];  // Assez grand pour stocker votre nombre entier

    // // Utilisation de sprintf pour formater la chaîne
    // sprintf(buffer, "%d", getPort());
    // std::string key = buffer;
    logs("LOG", "Starting server on 127.0.0.1:" + intToString(getPort()));
}


void    Server::initSocket(void) {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        throw std::runtime_error(std::string(ERROR) + "[ERROR]:\t    " + RESET + "Socket creation failed\n");
    }
}

void    Server::initAddress(void)
{
    std::memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
}

void    Server::initReadFds(void)
{
    FD_ZERO(&_readFds);
    FD_SET(_socket, &_readFds);
    _maxReadFds = _socket + 1;
}

// Main functions

void Server::run() {
    logs("LOG", "Waiting for connections...");

    while (true) {
        fd_set readFds;
        FD_ZERO(&readFds);

        FD_SET(_socket, &readFds);
        for (std::vector<User *>::const_iterator it = _users.begin(); it != _users.end(); ++it) {
            FD_SET((*it)->getSocket(), &readFds);
        }

        int selectResult = select(_maxReadFds, &readFds, NULL, NULL, NULL);
        if (selectResult == -1) {
            throw std::runtime_error(std::string(ERROR) + "[ERROR]:\t    " + RESET + "Select failed\n");
        }

        for (int i = 0; i < _maxReadFds; ++i) {
            if (FD_ISSET(i, &readFds)) {
                if (i == _socket)
                    handleClientConnection();
                else
                    handleClientData(getUserBySocket(i));
            }
        }
    }
}

// Connection handling

void Server::handleClientConnection() {
    struct sockaddr_in clientAddress;
    socklen_t clientAddrSize = sizeof(clientAddress);

    int clientSocket = accept(_socket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddrSize);
    if (clientSocket < 0) {
        logs("ERROR", "Accepting connection failed");
        return;
    }
    FD_SET(clientSocket, &_readFds);
    if (clientSocket >= _maxReadFds) {
        _maxReadFds = clientSocket + 1;
    }
    User *user = new User(clientSocket, clientAddress);
    
    std::pair<std::string, int> read = readFromClientSocket(user->getSocket());
    if (read.first == "" && read.second == -1) {
        handleClientDisconnection(user);
        return;
    }
    std::string buffer = read.first;

    if (buffer != "") {
        if (buffer.substr(0, 6) == "CAP LS") {
             // if (buffer.size() == 7) {
            //     send(clientSocket, "CAP LS\r\n", 8, 0);
            // }
            if (handleClientConnectionIrssi(user, buffer) == false) {
                FD_CLR(clientSocket, &_readFds);
                close(clientSocket);
                delete user;
            }
        } else if (handleClientConnectionNc(user) == false) {
            // sendMessage("001", "Disconnecting...", user);
            user->sendMessage("001", "Disconnecting...");
            FD_CLR(clientSocket, &_readFds);
            close(clientSocket);
            delete user;
        }
    }
    FD_CLR(clientSocket, &_readFds);
}


bool Server::handleClientConnectionIrssi(User *user, std::string buffer)
{
    std::vector<std::string> commandArgs;
    std::vector<std::string> commandUserArgs;
    std::map<std::string, std::string> capLS = splitStringIrssi(buffer);

    commandUserArgs = splitCommandUser(capLS["USER"]);
    commandArgs.push_back("USER");
    commandArgs.insert(commandArgs.end(), commandUserArgs.begin(), commandUserArgs.end());
    if (commandUser(this, user, commandArgs) == false)
        return false;

    commandArgs.clear();
    commandArgs.push_back("NICK");
    commandArgs.push_back(capLS["NICK"]);
    if (commandNick(this, user, commandArgs) == false) 
        return false;

    commandArgs.clear();
    commandArgs.push_back("PASS");
    commandArgs.push_back(capLS["PASS"]);
    if (commandPass(this, user, commandArgs) == false)
        return false;

    connectClient(user);
    return true;
}

bool Server::handleClientConnectionNc(User *user) {
    
    std::string input;
    std::vector<std::string> commandArgs;
    std::vector<std::string> commandUserArgs;    
   
    // sendMessage("001", "USER Please enter username:", user);
    user->sendMessage("001", "USER Please enter username:");
    input = readFromClientSocket(user->getSocket()).first;
    commandUserArgs = splitCommandUser(trimString(input));
    commandArgs.push_back("USER");
    commandArgs.insert(commandArgs.end(), commandUserArgs.begin(), commandUserArgs.end());
    if (commandUser(this, user, commandArgs) == false)
        return false;
    
    // sendMessage("001", "NICK Please enter nickname:", user);
    user->sendMessage("001", "NICK Please enter nickname:");
    input = readFromClientSocket(user->getSocket()).first;
    commandUserArgs = splitCommandUser(trimString(input));
    commandArgs.clear();
    commandArgs.push_back("NICK");
    commandArgs.insert(commandArgs.end(), commandUserArgs.begin(), commandUserArgs.end());
    if (commandNick(this, user, commandArgs) == false) 
        return false;

    // sendMessage("001", "PASS Please enter password:", user);
    user->sendMessage("001", "PASS Please enter password:");
    input = readFromClientSocket(user->getSocket()).first;
    commandUserArgs = splitCommandUser(trimString(input));
    commandArgs.clear();
    commandArgs.push_back("PASS");
    commandArgs.insert(commandArgs.end(), commandUserArgs.begin(), commandUserArgs.end());
    if (commandPass(this, user, commandArgs) == false)
        return false;
    
    connectClient(user);
    return true;
}

bool Server::connectClient(User *user) {
    if (user->isConnected() == true) {
        // sendMessage("462", "Unauthorized command (already registered)", user);
        user->sendMessage("462", "Unauthorized command (already registered)");
        logs("ERROR", "User :" + user->getCompleteName() + " already connected or not accepted");
        return false;
    }
    else if (user->isAccepted() == false) {
        // sendMessage("462", "Unauthorized command (not accepted)", user);
        user->sendMessage("462", "Unauthorized command (not accepted)");
        logs("ERROR", "User :" + user->getCompleteName() + " not accepted");
        return false;
    }
    User *tmp = getUserByCompleteName(user->getCompleteName());
    if (tmp != NULL)
        handleClientDisconnection(tmp);
    logs("LOG", "New connection from " + std::string(inet_ntoa(user->getAddress().sin_addr)) + ":" + intToString(ntohs(user->getAddress().sin_port)));
    addUser(user);
    user->setConnected(true);
    // sendMessage("001", "Welcome to Internet Relay Network !", user);
    user->sendMessage("001", "Welcome to Internet Relay Network !");
    return true;
}

// Disconnection handling

void Server::handleClientDisconnection(User *user) {
    FD_CLR(user->getSocket(), &_readFds);
    if (user->getSocket() != -1)
        close(user->getSocket());
    logs("LOG", "User :" + user->getCompleteName() + " disconnected");
    removeUser(user);
   
}

// Read data handling

void Server::handleClientData(User *user) {
    std::pair<std::string, int> read = readFromClientSocket(user->getSocket());
    if (read.first == "" && read.second == -1) {
        handleClientDisconnection(user);
        return;
    }
    std::string buffer = trimString(read.first);
    std::vector<std::string> commandArgs = splitString(buffer, ' ');

    // if command args first == "NICK"
    if (commandArgs[0] == "JOIN") {
        if (commandJoin(this, user, commandArgs) == false)
            return;
    }
    else if (commandArgs[0] == "KICK") {
        if (commandKick(this, user, commandArgs) == false)
            return;
    }
    else if (commandArgs[0] == "INVITE") {
        if (commandInvite(this, user, commandArgs) == false)
            return;
    }
    else if (commandArgs[0] == "TOPIC") {
        if (commandTopic(this, user, commandArgs) == false)
            return;
        std::cout << "commandArgs[2].substr(1) = " << commandArgs[2].substr(1) << std::endl;
        std::cout << "Fin commande Topic: Channel.Topic = " << this->getChannelByName(commandArgs[1].substr(1))->getName() << std::endl;
    }
    else if (commandArgs[0] == "MODE"){
        if (commandMode(this, user, commandArgs) == false)
            return;
    }
    else if (commandArgs[0] == "PRIVMSG") {
        
        std::string channelName = buffer.substr(9, buffer.find(' ', 9) - 9);
        std::string message = buffer.substr(buffer.find(' ', 9) + 1);
        sendPrivateMessage(user, channelName, message);
    }
    std::cout << "fin handle data" << std::endl;
    return;
}

// Tools

std::pair<std::string, int> Server::readFromClientSocket(int socket) {
    while (true) {
        char buffer[4096];
        ssize_t bytesRead = recv(socket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            return std::make_pair("", -1);
        }
        
        if (bytesRead > 0) {

            buffer[bytesRead] = '\0';
            std::cout << buffer << std::endl;
            std::vector<std::string> split = splitString(std::string(buffer), '\n');
            // std::cout << "true" << std::endl;
            for (int idx = 0; idx < (int)split.size(); idx++) {
                logs("RECEIVED", trimString(split[idx]));
                // std::cout << "true" << std::endl;

            }
            // std::cout << "true" << std::endl;

            std::string read = std::string(buffer, bytesRead);
            return std::make_pair(read, bytesRead);
        }
    }
    return std::make_pair("", -1);
}

// User management

User    *Server::addUser(User *user) {
    _users.push_back(user);
    logs("LOG", "User :" + user->getCompleteName() + " added");
    return user;
}

void Server::removeUser(User* user) {
    std::vector<User*>::iterator it = std::find(_users.begin(), _users.end(), user);
    if (it != _users.end()) {
        logs("LOG", "User :" + user->getCompleteName() + " removed");
        _users.erase(it);
        delete user;
    }
}

void Server::removeUserByUsername(const std::string& username) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getUsername() == username) {
            logs("LOG", "User :" + getUsers()[i]->getCompleteName() + " removed");
            getUsers().erase(getUsers().begin() + i);
            break;
        }
    }
}

void Server::removeUserByNickname(const std::string& nickname) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getNickname() == nickname) {
            logs("LOG", "User :" + getUsers()[i]->getCompleteName() + " removed");
            getUsers().erase(getUsers().begin() + i);
            break;
        }
    }
}

void Server::removeUserBySocket(int sock) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getSocket() == sock) {
            logs("LOG", "User :" + getUsers()[i]->getCompleteName() + " removed");
            getUsers().erase(getUsers().begin() + i);
            break;
        }
    }
}

User* Server::getUserByUsername(const std::string& username) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getUsername() == username) {
            return getUsers()[i];
        }
    }
    return NULL;
}

User* Server::getUserByNickname(const std::string& nickname) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getNickname() == nickname) {
            return getUsers()[i];
        }
    }
    return NULL;
}

User* Server::getUserBySocket(int sock) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getSocket() == sock) {
            return getUsers()[i];
        }
    }
    return NULL;
}

User *Server::getUserByAddress(const struct sockaddr_in& addr) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getAddress().sin_addr.s_addr == addr.sin_addr.s_addr && getUsers()[i]->getAddress().sin_port == addr.sin_port) {
            return getUsers()[i];
        }
    }
    return NULL;
}

User *Server::getUserByCompleteName(const std::string& completeName) {
    for (int i = 0; i < (int)getUsers().size(); i++) {
        if (getUsers()[i]->getCompleteName() == completeName) {
            return getUsers()[i];
        }
    } 
    return NULL;   
}

// Channel management

// void Server::createChannel(const std::string& channelName) {
//     getChannels().push_back(new Channel(channelName));
// }

Channel *Server::createChannel(const std::string& channelName, User* user) {
    Channel *channel = new Channel(channelName, user);
    std::cout << "Channel name create: " << channel->getName() << std::endl;
    std::cout << "Channel operator is: " << channel->getFirstOperator()->getUsername() << std::endl;
    channel->joinOperator(user);
   
    _channels.push_back(channel);
   
    std::cout << "_channels = " << _channels.front()->getFirstOperator()->getUsername() << std::endl;
    return channel;
}

// void Server::joinChannel(User* user, const std::string& channelName) {
//     Channel* channel = getChannelByName(channelName);
   
//     if (channel != nullptr) {
//         std::cout << "DANS" << std::endl;
//         channel->addUser(user);
//         logs("LOG", user->getNickname() + " joined channel " + channel->getName());
//         sendMessage("JOINCHANNEL", "Joined channel " + channel->getName(), user);
//     } else {
//         createChannel(channelName);
//         channel = getChannelByName(channelName);
//         if (channel != nullptr) {
//             channel->addUser(user);
//             logs("LOG", user->getNickname() + " joined channel the" + channel->getName());
//             sendMessage("JOINCHANNEL", "Joined channel " + channel->getName(), user);
//         } else {
//             std::cerr << "Failed to create or join channel: " << channelName << std::endl;
//         }
//     }
// }


void Server::leaveChannel(User* user, const std::string& channelName) {
    Channel* channel = getChannelByName(channelName);
    if (channel) {
        channel->removeUser(user);
        logs("LOG", user->getNickname() + " left channel " + channel->getName());
        // sendMessage("LEAVECHANNEL", "Left channel " + channel->getName(), user);
        user->sendMessage("LEAVECHANNEL", "Left channel " + channel->getName());
    }
}

void Server::sendPrivateMessage(User* sender, const std::string& channelName, const std::string& message) {
    Channel* channel = getChannelByName(channelName);
    if (channel) {
        for (int i = 0; i < (int)channel->getUsers().size(); i++) {
            std::cout << "SENDING MESSAGE TO " << channel->getUsers()[i]->getNickname()  << " FROM " << sender->getNickname() << std::endl;
            std::string msg = ":" + sender->getNickname() + " PRIVMSG #" + channelName + " " + message + "\r\n";
            std::cout << "Message: " << msg << std::endl;
            if (channel->getUsers()[i]->getCompleteName() != sender->getCompleteName())
                send(channel->getUsers()[i]->getSocket(), msg.c_str(), msg.size(), 0);
            std::cout << "fin send message" << std::endl;
        }
    }
    else {
        std::cerr << "Channel " << channelName << " not found" << std::endl;
    }
}



Channel* Server::getChannelByName(const std::string& channelName) {
    for (int i = 0; i < (int)_channels.size(); i++) {
        if (_channels[i]->getName() == channelName) {
            return _channels[i];
        }
    }
    return NULL; 
}

// Getters

int Server::getPort() const { return _port; }

std::string Server::getPassword() const { return _password; }

int Server::getSocket() const { return _socket; }

struct sockaddr_in Server::getAddress() const { return _address; }

std::vector<int>* Server::getClients() const { return _clients; }

std::vector<User *> Server::getUsers() const { return _users; }

std::vector<Channel*> Server::getChannels() const { return _channels; }

// Setters

void    Server::setPassword(const std::string& password) { _password = password; }

void    Server::setPort(int port)
{ 
    if (port < 0 || port > 65535)
        throw std::runtime_error(std::string(ERROR) + "[ERROR]:\t    " + RESET + "Port must be between 0 and 65535");
    _port = port;
}

void   Server::setClients(std::vector<int>* clients) { _clients = clients; }
