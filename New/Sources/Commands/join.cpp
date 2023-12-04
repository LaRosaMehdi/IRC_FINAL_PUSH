/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:19:39 by dojannin          #+#    #+#             */
/*   Updated: 2023/11/30 19:23:01 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"

bool commandJoin(Server* server, User* user, std::vector<std::string> args) {
    Channel* channel = nullptr;

    if (!user->isConnected()) {
        server->sendMessage("451", "You have not registered", user);
        logs("LOG", "JOIN :" + user->getCompleteName() + " (not registered)");
        return false;
    } else if (args.size() != 2 || args[1] == "") {
        std::string errorMessage = ":ft_irc 461 " + user->getNickname() + " JOIN :Not enough parameters\r\n";
        send(user->getSocket(), errorMessage.c_str(), errorMessage.size(), 0);

        logs("LOG", "JOIN :" + user->getCompleteName() + " (not enough parameters)");
        return false;
    }

    channel = server->getChannelByName(args[1].substr(1));
    if (channel != NULL) {
        channel->joinUser(user);
    } else {
        channel = server->createChannel(args[1].substr(1), user);
        if (channel == NULL) {
            server->sendMessage("400", "Failed to create channel " + args[1].substr(1), user);
            logs("LOG", "JOIN :" + user->getCompleteName() + " (failed to create channel " + args[1].substr(1) + ")");
            return false;
        }
        channel->joinUser(user);
        // std::string messageAddUser = ":" + user->getCompleteName() + " JOIN #" + channel->getName() + "\r\n";
        // send(user->getSocket(), messageAddUser.c_str(), messageAddUser.size(), 0);    
    } 
    if (channel->getTopic() == "") {
        server->sendChannelMessage("332", "No topic is set", channel, user);
        logs("LOG", "JOIN :" + user->getCompleteName() + " (no topic is set)");
    } else {
        server->sendChannelMessage("332", channel->getTopic(), channel, user);
        logs("LOG", "JOIN :" + user->getCompleteName() + " (topic is set)");
    }

    // user list
    std::string messageUserList = ":ft_irc 353 " + user->getNickname() + " = #" + channel->getName() + " :" + channel->getUsersList() + "\r\n";
    send(user->getSocket(), messageUserList.c_str(), messageUserList.size(), 0);

    std::string messageEndOfNames = ":ft_irc 366 " + user->getNickname() + " #" + channel->getName() + " :End of /NAMES list\r\n";
    send(user->getSocket(), messageEndOfNames.c_str(), messageEndOfNames.size(), 0);
    return true;
}

