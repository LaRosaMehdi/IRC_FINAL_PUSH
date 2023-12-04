/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/11/30 19:21:50 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "./Server.hpp"
#include "./User.hpp"
#include "./utils.hpp"

class Server;

bool    commandPass(Server *server, User *user, std::vector<std::string> args);
bool    commandNick(Server *server, User *user, std::vector<std::string> args);
bool    commandUser(Server *server, User *user, std::vector<std::string> args);
bool    commandJoin(Server *server, User *user, std::vector<std::string> args);
bool    commandKick(Server *server, User *user, std::vector<std::string> args);

#endif
