/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:19:39 by dojannin          #+#    #+#             */
/*   Updated: 2023/12/21 18:15:37 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"

/*
    :server.example.com 473 yourNick #channel :Cannot join channel (+i)
    :inviterNick!inviterUser@inviter.host.com 341 yourNick #channel
*/

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
        if (channel->getInviteBool() == true){ 
            std::cout << "tu reeeentres" << std::endl;
            if (!channel->isInvitedByOperator(user)){
                user->sendMessage("473", "-!- Irssi: Cannot join channel");
                logs("LOG", "JOIN :" + user->getCompleteName() + " (cannot join channel " + args[1].substr(1) + ")");
                return false;
            }
        }
        else if (channel->getPasswordRestriction() == true){
            if (args.size() == 3){
                if (args[2] == channel->getPassword()){
                    channel->joinUser(user);   
                }
                else{
                    user->sendMessage("475", "-!- Irssi: Cannot join channel (+k)");
                    logs("LOG", "JOIN :" + user->getCompleteName() + " (cannot join channel " + args[1].substr(1) + ")");
                    return false;
                }
            }
            else{
                // :serveur-irc 475 votre-pseudo #nom-du-canal :Cannot join channel (+k)
                user->sendMessage("475", "-!- Irssi: Cannot join channel (+k)");
                logs("LOG", "JOIN :" + user->getCompleteName() + " (cannot join channel " + args[1].substr(1) + ")");
                return false;
            }
        }
        else
            channel->joinUser(user);
    } 
    else 
    {
        std::cout << "weeeee" << std::endl;
        channel = server->createChannel(args[1].substr(1), user);
        std::cout << channel->getFirstOperator()->getUsername() << std::endl;
        if (channel == NULL) {
            user->sendMessage("400", "Failed to create channel " + args[1].substr(1));
            logs("LOG", "JOIN :" + user->getCompleteName() + " (failed to create channel " + args[1].substr(1) + ")");
            return false;
        }
        if (channel->getInviteBool() == true){ 
            std::cout << "tu reeeentres" << std::endl;
            if (!channel->isInvitedByOperator(user)){
                user->sendMessage("473", "-!- Irssi: Cannot join channel");
                logs("LOG", "JOIN :" + user->getCompleteName() + " (failed to create channel " + args[1].substr(1) + ")");
                return false;
            }
        }
        std::cout << "meme pas et cest pour ca" << std::endl;
        channel->joinUser(user);
        channel->joinOperator(user);
        // server.
    } 
    std::cout << "je sais je sais " << std::endl;
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

