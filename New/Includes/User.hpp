/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/11/30 19:28:28 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "./utils.hpp"

class User {

    private:
        std::string         _username;
        std::string         _nickname;
        std::string         _realname;
        std::string         _hostname;

        // int                 _socket;
        int                 _fd;
        struct sockaddr_in  _address;
        bool				_accepted;
        bool				_connected;
        bool                _checkpwd;
        bool                _checknick;
        bool                _checkuser;

    public:

        User(const int socket, const struct sockaddr_in& addr);
        User(const std::string& username, const std::string& nickname, int sock, const struct sockaddr_in& addr);
        User(const std::string& username, const std::string& nickname, int sock, const struct sockaddr_in& addr, bool accepted, bool connected);
        ~User();

        // Getters
        std::string getUsername() const;
        std::string getNickname() const;
        std::string getRealname() const;
        std::string getHostname() const;
        std::string getCompleteName() const;
        struct sockaddr_in getAddress() const;
        int getSocket() const;

        // Setters
        void setUsername(const std::string& username);
        void setNickname(const std::string& nickname);
        void setRealname(const std::string& realname);
        void setHostname(const std::string& hostname);
        void setAddress(const struct sockaddr_in& address);
        void setSocket(int socket);
        void setAccepted(bool accepted);
        void setConnected(bool connected);

        // Is
        bool isAccepted() const;
        bool isConnected() const;

        // Send message 
        bool sendMessage(std::string code, std::string message);
        std::string formatSendMessage(std::string code, std::string message);

        // For commands AKA Do
        bool                isCheckPwd() const;
        bool                isCheckNick() const;
        bool                isCheckUser() const;

        // changes states of the bool of the client during process
        void                switchPwd(void) ;
        void                switchNick(void) ;
        void                switchUser(void) ;
        
};

#endif
