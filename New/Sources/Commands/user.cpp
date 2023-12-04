/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:39:34 by dojannin          #+#    #+#             */
/*   Updated: 2023/11/30 19:22:47 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"

//Peut rajouter des id/bool pour user, nick et pass, et valider

// void    cmdUser(Server& ser, User *user, std::vector<std::string> input){
//     (void)ser;
// 	user->setUsername(input[1]);
//     user->switchUser();
// 	std::cout << "\e[1;36mUser set\e[0m" << std::endl;
// 	//  Checker etat user pour log 
// }

bool    commandUser(Server *server, User *user, std::vector<std::string> args) {
	
	if (user->isConnected() == true) {
		server->sendMessage("462", "Unauthorized command (already registered)", user);
		logs("LOG", "USER :" + user->getCompleteName() + " (already registered)");
		return false;
	}
	else if (args.size() != 5) {
		server->sendMessage("461", "Not enough parameters", user);
		logs("LOG", "USER :" + user->getCompleteName() + " (not enough parameters)");
		return false;
	}
	user->setUsername(args[1]);
	user->setHostname(args[2]);
	user->setRealname(args[4]);
	logs("LOG", "USER :" + user->getCompleteName() + " (accepted)");
	return true;
}
