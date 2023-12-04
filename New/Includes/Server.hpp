/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/11/30 19:28:19 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <map>  // Added for std::map
#include <utility>

#include "./User.hpp"
#include "./utils.hpp"
#include "./Channels.hpp"  // Added for Channel class
#include "./Commands.hpp"

extern int defaultUserIndex;

class Server
{
    private:
        int _port;
        std::string _password;
        int _socket;
        struct sockaddr_in _address;

        // Users
        std::vector<User *> _users;
        std::vector<int>* _clients;
        fd_set _readFds;
        int _maxReadFds;
        std::vector<Channel*> _channels;

    public:
        Server(int port, const std::string& password);
        ~Server();

        // Main functions
        void run(void);

        void handleClientConnection(void);
        bool handleClientConnectionNc(User *user);
        bool handleClientConnectionIrssi(User *user, std::string buffer);
        void handleClientDisconnection(User *user);
        void handleClientData(int socket);
        bool connectClient(User *user);

        // Utils
        bool sendMessage(std::string code, std::string message, User *user);
        std::string formatSendMessage(std::string code, std::string message, User *user);
        std::pair<std::string, int> readFromClientSocket(int socket);

        // User managements
        User *addUser(User *user);
        void removeUser(User *user);
        void removeUserByUsername(const std::string& username);
        void removeUserByNickname(const std::string& nickname);
        void removeUserBySocket(int sock);

        User *getUserByCompleteName(const std::string& completeName);
        User *getUserByUsername(const std::string& username);
        User *getUserByNickname(const std::string& nickname);
        User *getUserBySocket(int sock);
        User *getUserByAddress(const struct sockaddr_in& addr);

        // Inits
        void init(void);
        void initSocket(void);
        void initAddress(void);
        void initReadFds(void);

        // Getters
        int getPort() const;
        std::string getPassword() const;
        int getSocket() const;
        struct sockaddr_in getAddress() const;
        std::vector<int>* getClients() const;
        std::vector<User *> getUsers() const;

        // Setters
        void setPort(int port);
        void setPassword(const std::string& password);
        void setClients(std::vector<int>* clients);

        void createChannel(const std::string& channelName);
        void joinChannel(User* user, const std::string& channelName);
        void leaveChannel(User* user, const std::string& channelName);
        void sendMessageToChannel(User* sender, const std::string& channelName, const std::string& message);
        Channel* getChannelByName(const std::string& channelName);
};

#endif
