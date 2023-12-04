/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mla-rosa <mla-rosa@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/11/30 19:23:28 by mla-rosa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "./../Includes/Server.hpp"
#include "./../Includes/utils.hpp"

int main(int ac, char **av) {
    Server* server = nullptr;
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
