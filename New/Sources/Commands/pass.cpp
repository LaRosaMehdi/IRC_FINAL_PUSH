/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:20:03 by dojannin          #+#    #+#             */
/*   Updated: 2023/11/30 19:22:49 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../Includes/Server.hpp"

// void    cmdPass(Server& ser, User *user, std::string input){
//     std::cout << "est ce que tu rentres" <<std::endl;
// 	std::cout << "serv.getPassword = " << ser.getPassword() << std::endl; 
// 	if (user->isAccepted() == true){
// 		std::cout << "tombe ici le chacal" << std::endl;
// 		return ;
// 	}
// 	else if (input == ser.getPassword()) {
// 		std::cout << "1" << std::endl;
// 		// user->setPassword(input);
// 		user->switchPwd();
// 		std::cout << "\e[1;36mPassword set\e[0m" << std::endl;
// 	}
// 	else if (input != ser.getPassword() && user->isCheckPwd() == true) {
// 		std::cout << "ici " << std::endl;
// 		std::string error = "462 :Unauthorized command (already registered)\r\n";
// 		std::cout << error << std::endl;
// 		send(user->getSocket(), error.c_str(), error.size(), 0);
// 	}
//     //check etat pour log
// 	return ;
// }

bool    commandPass(Server *server, User *user, std::vector<std::string> args) {
	if (user != NULL && user->isConnected() == true) {
		user->sendMessage("462", "Unauthorized command (already registered)");
		logs("LOG", "PASS :" + user->getCompleteName() + " (already registered)");
		return false;
	}
	else if (args.size() != 2) {
		user->sendMessage("461", "Not enough parameters");
		logs("LOG", "PASS :" + user->getCompleteName() + " (not enough parameters)");
		return false;
	}
	else if (args[1] != server->getPassword()) {
		user->sendMessage("464", "Password incorrect\n");
		logs("LOG", "PASS :" + user->getCompleteName() + " (incorrect password)");
		return false;
	}
	user->setAccepted(true);
	logs("LOG", "PASS :" + user->getCompleteName() + " (accepted)");
	return true;
}
