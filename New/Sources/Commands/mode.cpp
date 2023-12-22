/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 16:15:40 by dojannin          #+#    #+#             */
/*   Updated: 2023/12/22 15:57:56 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"
#include "./../../Includes/Channel.hpp"


/*MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel
operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel
*/

bool commandMode(Server* server, User* user, std::vector<std::string> args) {

    if (args.size() < 2) {
        std::cout << "Usage: /MODE <channel> <mode> [parameter]" << std::endl;
        return false;
    }
    std::string channelName = args[1].substr(1);
    std::string mode = args[2];
    Channel* channel = server->getChannelByName(channelName);
    if (!channel) {
        std::cout << "Channel '" << channelName << "' not found." << std::endl;
        return false;
    }
    if (channel->getOperatorByUsername(user->getUsername())){
        if (mode == "+i" || mode == "-i") {
            // i: Set/remove Invite-only channel
            if (mode == "+i"){
                std::cout << "Channel is protected" << std::endl;
                channel->setInviteOnly(true);
            }
            else if (mode == "-i"){
                channel->setInviteOnly(false);
            }
        }
        else if (mode == "+t" || mode == "-t") {
            // t: Set/remove the restrictions of the TOPIC command to channel operators
            if (mode == "+t"){
                std::cout << "Channel is protected" << std::endl;
                channel->setTopicRestriction(true);
            }
            else if (mode == "-t"){
                channel->setTopicRestriction(false);
            }
        }
        else if (mode == "+k" || mode == "-k") {
            // k: Set/remove the channel key (password)
            if (args.size() >= 2) {
                if (mode == "+k"){
                    std::string channelKey = args[3];
                    channel->setChannelPass(channelKey);
                }
                else if (mode == "-k"){
                    channel->removeChannelPass();
                }
            }
            else {
                std::cout << "Missing channel key parameter." << std::endl;
                return false;
            }
        }
        else if (mode == "+o" || mode == "-o") {
            // o: Give/take channel operator privilege
            if (args.size() > 2) {
                std::string targetUserName = args[3];
                User* targetUser = server->getUserByUsername(targetUserName);

                if (targetUser) {
                    if (mode == "+o"){
                    bool giveOperatorPrivilege = true;
                    channel->giveTakeOperator(targetUser, giveOperatorPrivilege);
                      }
                    else if (mode == "-o"){
                        channel->giveTakeOperator(targetUser, false);
                        }
                }
                else {
                    std::cout << "User '" << targetUserName << "' not found." << std::endl;
                    return false;
                }
            } 
            else {
                std::cout << "Missing target user parameter." << std::endl;
                return false;
            }
        // } 
        // else if (mode == "l") {
        //     // l: Set/remove the user limit to channel
        //     if (args.size() > 2) {
        //         int userLimit = std::stoi(args[2]);
        //         channel->setUserLimit(userLimit);
        //     } else {
        //         std::cout << "Missing user limit parameter." << std::endl;
        //         return false;
        //     }
        // } 
        // else {
        //     // Mode non reconnu, afficher un message d'erreur
        //     std::cout << "Unknown mode '" << mode << "'." << std::endl;
        //     return false;
        }
    }
    else
    {
        user->sendMessage("482", "-!- Irssi: You're not channel operator");
        return false;
    }

    // La commande MODE a été traitée avec succès
    return true;
}
