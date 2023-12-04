/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/11/30 19:21:36 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <vector>
#include <algorithm>
#include <string>
#include "Server.hpp"
#include "User.hpp"

class Channel {
    public:
        Channel(const std::string& name, User *op) : _name(name), _operator(op) {
            setTopic("");
        }
        ~Channel() {}
        std::string getName() const {
        return _name;
        }
        bool    removeUser(User* user) {
            std::vector<User*>::iterator it = std::find(_users.begin(), _users.end(), user);
            if (it != _users.end()) {
                _users.erase(it);
                return true;
            }
            return false;
        }
        void sendBroadcastMessage(const std::string& message, User* sender)
        {
            std::cout << "sendBroadcastMessageing message: " << message << std::endl;
            std::vector<User*>::iterator it = _users.begin();
            while (it != _users.end()) {
                if (*it != sender) {
                (*it)->sendRawMessage(message);
                }
                it++;
            }
        }
        void joinUser(User* user)
        {
            _users.push_back(user);
            sendBroadcastMessage(":" + user->getCompleteName() + " JOIN #" + getName() + "\r\n", user);
        }
        std::vector<User *> getUsers() const {
            return _users;
        }

        std::string getTopic() const {
            return _topic;
        }

        void setTopic(const std::string& topic) {
            _topic = topic;
        }

        bool isOperator(User* user) const {
            return user == _operator;
        }

        User *getOperator() const {
            return _operator;
        }

        void setOperator(User *op) {
            _operator = op;
        }

        std::string	getUsersList(void)
        {
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

        User* getUserByUsername(const std::string& username) {
            for (int i = 0; i < (int)getUsers().size(); i++) {
                if (getUsers()[i]->getUsername() == username) 
                    return getUsers()[i];         
            }
            return NULL;
        }
    
        
    private:
        std::string _name;
        std::string _topic;
        std::vector<User*> _users;
        std::vector<std::string> _messages;
        User *_operator;
};

#endif /* !CHANNELS_HPP */

