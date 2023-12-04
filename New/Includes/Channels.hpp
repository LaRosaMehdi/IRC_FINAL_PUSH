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
        Channel(const std::string& name) : _name(name) {}
        ~Channel() {}
        std::string getName() const {
        return _name;
        }
        void removeUser(User* user) {
        std::vector<User*>::iterator it = std::find(_users.begin(), _users.end(), user);
        if (it != _users.end()) {
            _users.erase(it);
        }
        }
        void sendMessage(const std::string& message, User* sender)
        {
        std::vector<User*>::iterator it = _users.begin();
        while (it != _users.end()) {
            if (*it != sender) {
            (*it)->sendRawMessage(message);
            }
            it++;
        }
        }
        void addUser(User* user)
        {
            _users.push_back(user);
        }
        std::vector<User *> getUsers() const {
            return _users;
        }
        
    private:
        std::string _name;
        std::vector<User*> _users;
        std::vector<std::string> _messages;
};

#endif /* !CHANNELS_HPP */

