/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/11/30 19:21:36 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <algorithm>
#include <string>
#include "User.hpp"

class User;

class Channel {
    public:
        Channel(const std::string& name, User *op);
        ~Channel();
        
        // User management
        bool    removeUser(User* user);
        void    joinUser(User* user);
        void    joinOperator(User* user);
        
        // Message management
        void sendBroadcastMessage(const std::string& message, User* sender);
        bool    sendChannelMessage(std::string code, std::string message, User *user, bool userList = false);
        std::string formatSendChannelMessage(std::string code, std::string message, User *user, bool userList = false);

        // Getters
        std::string getName() const;
        std::vector<User *> getUsers() const;
        std::string getTopic() const;
        User *getFirstOperator() const;
        std::vector<User *> getOperator() const;
        std::string	getUsersList(void);
        User* getUserByUsername(const std::string& username);
        User* getOperatorByUsername(const std::string& username);
        // Setters
        void setName(const std::string& name);
        void setTopic(const std::string& topic);
        void setOperator(User* op);
        void clearTopic(void);


        // Is
        bool isOperator(User* user) const;
        
    private:
        std::string _name;
        std::string _topic;
        std::vector<User*> _users;
        std::vector<std::string> _messages;
        std::vector<User*> _operator;
};

#endif /* !CHANNELS_HPP */

