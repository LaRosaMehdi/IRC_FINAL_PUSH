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
    if (channel != nullptr) {
        channel->addUser(user);
         std::cout << "JOIN channel: " << channel->getName() << std::endl;
        server->sendMessage("JOIN", "Joined channel " + channel->getName(), user);
        logs("LOG", "JOIN :" + user->getCompleteName() + " (joined channel " + channel->getName() + ")");
    } else {
        channel = server->createChannel(args[1].substr(1));
        if (channel == nullptr) {
            server->sendMessage("400", "Failed to create channel " + args[1].substr(1), user);
            logs("LOG", "JOIN :" + user->getCompleteName() + " (failed to create channel " + args[1].substr(1) + ")");
            return false;
        }

        if (channel->getTopic() == "")
            server->sendMessage("332", channel->getName() + " :No topic is set", user);
        else
            server->sendMessage("332", channel->getName() + " :" + channel->getTopic(), user);

        server->sendMessage("353", "= " + channel->getName() + " :" + channel->getUsersList(), user);
        server->sendMessage("366", channel->getName() + " :End of /NAMES list", user);

        channel->addUser(user);
        std::cout << "JOIN channel: " << channel->getName() << std::endl;
        server->sendMessage("JOIN", "Joined channel " + channel->getName(), user);
        logs("LOG", "JOIN :" + user->getCompleteName() + " (joined channel " + channel->getName() + ")");
    }
    return true;
}

