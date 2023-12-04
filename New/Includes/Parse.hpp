/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by dojannin          #+#    #+#             */
/*   Updated: 2023/11/30 19:21:56 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
#define PARSE_HPP

#include "Server.hpp"
#include "User.hpp"
#include "utils.hpp"

class Server;
//class User;

#include <vector>

class   Parse
{
    private:
        std::string                 cmd;
        std::vector<std::string>    tab;
    public:
        Parse(void);
        ~Parse(void);
        int     parseirssi(Server&, std::string);
        void    parsecmd(Server&, User *, int, std::string);
};

#endif
