/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:19:39 by dojannin          #+#    #+#             */
/*   Updated: 2023/12/11 16:49:56 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"

bool commandJoin(Server* server, User* user, std::vector<std::string> args) {
    Channel* channel = NULL;

    if (!user->isConnected()) {
        user->sendMessage("451", "You have not registered");
        logs("LOG", "JOIN :" + user->getCompleteName() + " (not registered)");
        return false;
    } else if (args.size() != 2 || args[1] == "") {
        user->sendMessage("461", "Not enough parameters");
        logs("LOG", "JOIN :" + user->getCompleteName() + " (not enough parameters)");
        return false;
    }

    channel = server->getChannelByName(args[1].substr(1));
    if (channel != NULL) {
        channel->joinUser(user);
    } else {
        channel = server->createChannel(args[1].substr(1), user);
        std::cout << channel->getFirstOperator()->getUsername() << std::endl;
        if (channel == NULL) {
            user->sendMessage("400", "Failed to create channel " + args[1].substr(1));
            logs("LOG", "JOIN :" + user->getCompleteName() + " (failed to create channel " + args[1].substr(1) + ")");
            return false;
        }
        channel->joinUser(user);
        channel->joinOperator(user);
        // server.
    } 
    if (channel->getTopic() == "") {
        channel->sendChannelMessage("332", "No topic is set", user);
        logs("LOG", "JOIN :" + user->getCompleteName() + " (no topic is set)");
    } else {
        channel->sendChannelMessage("332", channel->getTopic(), user);
        logs("LOG", "JOIN :" + user->getCompleteName() + " (topic is set)");
    }
    channel->sendChannelMessage("353", channel->getUsersList(), user, true);
    channel->sendChannelMessage("366", "End of /NAMES list", user);
    logs("LOG", "JOIN :" + user->getCompleteName() + " (user list)");
    return true;
}

