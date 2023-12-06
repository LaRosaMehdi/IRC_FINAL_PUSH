/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/12/06 17:00:47 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "./Server.hpp"
#include "./User.hpp"
#include "./utils.hpp"

class Server;
class User;

bool    commandPass(Server *server, User *user, std::vector<std::string> args);
bool    commandNick(Server *server, User *user, std::vector<std::string> args);
bool    commandUser(Server *server, User *user, std::vector<std::string> args);
bool    commandJoin(Server *server, User *user, std::vector<std::string> args);
bool    commandKick(Server *server, User *user, std::vector<std::string> args);
bool    commandInvite(Server *server, User *user, std::vector<std::string> args);

#endif
