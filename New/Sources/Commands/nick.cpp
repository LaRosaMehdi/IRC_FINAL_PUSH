/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:19:39 by dojannin          #+#    #+#             */
/*   Updated: 2023/11/30 19:23:01 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"

// void    cmdNick(Server &ser, User * user, std::string input){
//     for (size_t i = 0; i < ser.users.size(); i++) {
// 		if (ser.users[i]->getNickname() == input) {
// 			std::string ERR;
// 			ERR = "433 * " + input + " :Nickname is already in use" + "\r\n";
// 			send(user->getSocket(), ERR.c_str(), strlen(ERR.c_str()), 0);
// 			std::cout << ERR << std::endl;
// 			return ;
// 		}
// 	}
// 	user->setNickname(input);
//     user->switchNick();
// 	std::cout << "\e[1;36mNickname set\e[0m" << std::endl;
// 	// Checker etat final
// 	return ;
// }

bool	commandNickValid(std::string nick) {
	if (nick.size() > 9)
		return false;
	for (int idx = 0; idx < (int)nick.size(); idx++) {
		if (std::isalpha(nick[idx]) == false && std::isdigit(nick[idx]) == false)
			return false;
	}
	return true;
}

bool    commandNick(Server *server, User *user, std::vector<std::string> args) {
	
	if (args.size() != 2 && args[1].empty()) {
		server->sendMessage("431", "Not nickname given", user);
		logs("LOG", "NICK :" + user->getCompleteName() + " (not enough parameters)");
		return false;
	}
	else if (commandNickValid(args[1]) == false) {
		server->sendMessage("432", "Erroneous nickname", user);
		logs("LOG", "NICK :" + user->getCompleteName() + " (erroneous nickname)");
		return false;
	}
	else if (server->getUserByNickname(args[1]) != NULL) {
		server->sendMessage("433", "Nickname is already in use", user);
		logs("LOG", "NICK :" + user->getCompleteName() + " (nickname already in use)");
		return false;
	}
	logs("LOG", "NICK :" + user->getCompleteName() + " (updated to " + args[1] + ")");
	user->setNickname(args[1]);
	
	// !!!! NEEDS TO NOTIFY OTHER USERS IF IN CHANNELS OF NICK CHANGE
	return true;
}
