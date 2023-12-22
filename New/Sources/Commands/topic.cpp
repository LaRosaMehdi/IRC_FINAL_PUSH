/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 22:45:17 by dojannin          #+#    #+#             */
/*   Updated: 2023/12/21 16:55:26 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"
#include "./../../Includes/Channel.hpp"

bool	commandTopic(Server *serv, User *user, std::vector<std::string> arg) {
	std::string rpl;
	Channel * chan;
	if (arg.size() == 1) {
		/*
		? RPL_NEEDMOREPARAMS
		*/
		rpl = "461 TOPIC :Not enough parameters\r\n";
        std::cout << rpl;
    	send(user->getSocket(), rpl.c_str(), rpl.size(), 0);
		// chan->sendBroadcastMessage(rpl, user);

	}
	else if (arg.size() == 2) {
		std::cout << "|" << arg[1] << "|" << std::endl;
		chan = serv->getChannelByName(arg[1]);
		if (!chan) {
			/* 
			* No such channel.
			*/
			rpl = "403 " + arg[1] + " :No such channel\r\n";
            std::cout << rpl;
			user->sendMessage("403", rpl);
            send(user->getSocket(), rpl.c_str(), rpl.size(), 0);
		}
		else if (chan->getTopic().empty()) {
			/* 
			* RPL_NOTOPIC
			*/
			rpl = "332 " + user->getNickname() + " " + chan->getName() + " :No Topic set\r\n";
            std::cout << rpl;
			send(user->getSocket(), rpl.c_str(), rpl.size(), 0);
		}
		else if (chan->getTopic().empty() != 0) {
			/* 
			* RPL_TOPIC
			*/
			rpl = "332 " + user->getNickname() + " " + chan->getName() + " " + chan->getTopic() + "\r\n";
            std::cout << rpl;
			send(user->getSocket(), rpl.c_str(), rpl.size(), 0);
            return true;
		}
	}
	else if (arg.size() >= 3) {
		chan = serv->getChannelByName(arg[1].substr(1));
		if (!chan) {
			/* 
			* No such channel.
			*/
			std::cout << "cest toi ?" << std::endl;
			rpl = "403 " + arg[1] + " :No such channel\r\n";
            std::cout << rpl;
			send(user->getSocket(), rpl.c_str(), rpl.size(), 0); 
			serv->sendPrivateMessage(user, chan->getName(), rpl);
		}
		// std::cout << chan->getName() << std::endl;
        // std::cout << "See it" << std::endl;
        // std::cout << user->getCompleteName() << std::endl;
        // std::cout << "not see it" << std::endl;
		// std::cout << "try = " << serv->getChannelByName(chan->getName()) << std::endl;
		// std::cout << "chan = " << chan->getFirstOperator()->getUsername() << std::endl;
		if (chan){
			if (chan->isOperator(user) || !chan->getTopicRestriction()) {
            std::cout << "check" << std::endl;
			/* 
			! the user must be a superUser to set the topic.
			*/
			if (arg[2] == ":") {
				// Clear the Topic
				chan->clearTopic();
			}
			else {
				/*
				* Replace the current topic by the string [Topic].
				*/
				std::string topic;
				for (size_t i = 2; i < arg.size(); i++) {
					topic += arg[i];
					if (i != arg.size())
						topic += " ";
				}
				if (topic[0] == ':')
					topic = topic.substr(1);
				chan->setTopic(topic);
				rpl = ":" + user->getNickname() + "!" + user->getUsername() + "@127.0.0.1 TOPIC " + chan->getName() + " " + topic + "\r\n";
                std::cout << rpl;
				chan->sendBroadcastMessage(rpl, user);
				chan->sendChannelMessage("000", rpl, user, true);

				std::cout << "return true of topic fct" << std::endl;
                return true;
			}
			}
		}
		else {
			/*
			! The user isn't a superUser.
			*/
            if (!chan)
				return false;
			else {
				std::cout << "Isn't a superUtilisator" << std::endl;
				rpl = "482 " + chan->getName() + " :You're not channel operator\r\n";
				std::cout << rpl;
				chan->sendBroadcastMessage(rpl, user);
				send(user->getSocket(), rpl.c_str(), rpl.size(), 0);
				return false;
			}
		}
	}
    std::cout << "whyyyy" << std::endl;
    return false;
}