/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/12/06 14:21:05 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "./../Includes/Server.hpp"
#include "./../Includes/utils.hpp"

int main(int ac, char **av) {
    Server* server = NULL;
    if (ac != 3) {
        logs("ERROR", "Usage ./test <username> <nickname>");
        return EXIT_FAILURE;
    }
    try {
        server = new Server(atoi(av[1]), av[2]);
        signal(SIGINT, signalHandler);
        signal(SIGQUIT, signalHandler);
        server->run();
    } catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        delete server;
        return EXIT_FAILURE;
    }
    delete server;
    return EXIT_SUCCESS;
}
