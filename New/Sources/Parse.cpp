/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:13:57 by dojannin          #+#    #+#             */
/*   Updated: 2023/11/30 19:24:14 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../Includes/Parse.hpp"


Parse::Parse(void){
}

Parse::~Parse(void){
}

/*  
    Doit uniquement verifier si la phrase complete
    commence bien par CAP LS et possede apres chaque espace 
    un element PASS et sa valeur
// */
int Parse::parseirssi(Server& ser, std::string input){
    std::cout << "on se voit mon khey" << std::endl;
    std::string delim("\r\n");
    std::vector<std::string> tmp = splitVector(input, delim);
    std::cout << "Tmmp[0] = " << tmp[0] << std::endl;
    if (tmp[0] == "CAP LS"){
        std::vector<std::string>::const_iterator it = tmp.begin();
        for (; it != tmp.end(); it++){
            std::cout << "This is data = " << it->data() << std::endl;
            std::string stock = it->substr(0, (it->find(' ')));
            if (stock == "PASS"){
                if (it++ != tmp.end() && it->c_str() != ser.getPassword()){
                    return (0);
                }
            }
        }
    }
    return (1);
}

void    Parse::parsecmd(Server& serv, User *User, int clientIndex, std::string message){
    //Doit checker USER PASS NICK JOIN 
    (void)clientIndex;
    	std::string delim("\r\n");
        tab = splitVector(message, delim);
        if (tab.size() == 1) {
            delim = "\n";
            tab = splitVector(message, delim);
        }
        // std::cout << tab.size() << std::endl;
        for (size_t i = 0; i < tab.size(); i++) {
            if (tab[i].empty() == 0)
                std::cout << "\e[0;33m[MESSSAGE][" << i << "]\e[0m = " << "\e[0;32m{" << tab[i] << "}\e[0m" << std::endl;
        }
        delim = " ";
        for (size_t i = 0; i < tab.size(); i++) {
            std::vector<std::string> line = splitVector(tab[i], delim);
            if (line[0] == "NICK" && line.size() == 2)
                cmdNick(serv, User, line[1]);
                //commandNick(serv, User, line);
            else if (line[0] == "NICK" && line.size() == 1) {
                std::string error = "431 " + User->getNickname() + " :No nickname given\r\n";
                std::cout << error << std::endl;
                send(User->getSocket(), error.c_str(), strlen(error.c_str()), 0);
            }	
            else if (line[0] == "USER" && line.size() >= 5)
                cmdUser(serv, User, line);
            else if (line[0] == "USER" && line.size() <= 5) {
                std::string error = "461 USER :Not enough parameters\r\n";
                std::cout << error << std::endl;
                send(User->getSocket(), error.c_str(), strlen(error.c_str()), 0); 
            }
            else if (line[0] == "PASS" && line.size() < 2) {
                std::string error = "461 PASS :Not enough parameters\r\n";
                std::cout << error << std::endl;
                send(User->getSocket(), error.c_str(), error.size(), 0);
            }
            else if (line[0] == "PASS")
                cmdPass(serv, User, line[1]);
        }
        //
        //     /* If the client is accepted he can execute this commands otherwise he can't*/
        //     if (Client->accepted == true) {
        //         if (line[0] == "PING" && line.size() == 2) {
        //             std::string	to_send = ":irc PONG :" + line[1] + "\r\n";
        //             // colorMsg("\e[1;32m", (char *)to_send.c_str());
        //             rplDisplay(to_send);
        //             send(Client->getFd(), to_send.c_str(), strlen(to_send.c_str()), 0); 
        //         }
        //         else if (line[0] == "MODE")
        //             cmdMode(serv, Client, line);
        //         else if (line[0] == "JOIN")
        //             cmdJoin(serv, Client, line);
        //         else if (line[0] == "PRIVMSG" || line[0] == "NOTICE")
        //             cmdPrivMsg(serv, Client, line);
        //         else if (line[0] == "PART")
        //             cmdPart(serv, Client, line);
        //         else if (line[0] == "TOPIC")
        //             cmdTopic(serv, Client, line);
        //         else if (line[0] == "OPER")
        //             cmdOper(serv, Client, line);
        //         else if (line[0] == "kill")
        //             cmdKill(serv, Client, line);
        //         else if (line[0] == "KICK")
        //             cmdKick(serv, Client, line);
        //     }
		// }	
}