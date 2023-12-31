/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dojannin <dojannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 01:14:20 by mla-rosa          #+#    #+#             */
/*   Updated: 2023/12/06 14:56:43 by dojannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <signal.h>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "Server.hpp"
#include "./User.hpp"

// Utils
void                                signalHandler(int signum);

std::string                         trimString(const std::string& str);

std::vector<std::string>            splitVector(std::string s, std::string delimiter);
std::vector<std::string>            splitString(const std::string& str, char delimiter);
std::map<std::string, std::string>  splitStringIrssi(const std::string& input_str);
std::vector<std::string>            splitCommandUser(const std::string& input);

std::string                         intToString(int value);

// Logs
#define ERROR "\033[91m"
#define LOG "\033[94m"
#define RECEIVED "\033[92m"
#define RESET "\033[0m"

void        logs(std::string type, std::string message);



#endif
