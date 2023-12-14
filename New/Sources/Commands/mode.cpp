/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 16:15:40 by dojannin          #+#    #+#             */
/*   Updated: 2023/12/12 14:43:20 by dojannin         ###   ########.fr       */
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

bool commandMode(Server* server, User* user, std::vector<std::string> args){
	
}