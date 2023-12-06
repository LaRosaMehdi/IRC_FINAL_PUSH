#include <iostream>
#include "./../Includes/utils.hpp"

void signalHandler(int signum) {
    if (signum == SIGINT || signum == SIGQUIT) {
         throw std::runtime_error(std::string(LOG) + "[LOG]:        " + RESET + "Server ended\n");
    }
}

std::map<std::string, std::string> splitStringIrssi(const std::string& input_str) {
    std::map<std::string, std::string> parsed_data;
    std::string input = input_str.substr(8);
    std::string key, value;
    int i = 0;

    input.erase(std::remove(input.begin(), input.end(), '\r'), input.end());
    while (i < (int)input.length()) {
        key = "";
        value = "";

        while (input[i] != ' ') {
            key += input[i++];
        }
        i++;
        while (input[i] != '\n' && input[i] != '\0') {
            value += input[i++];
        }
        i++;
        parsed_data[key] = value;
    }
    return parsed_data;
}

std::vector<std::string> splitVector(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        parts.push_back(token);
    }
    
    return parts;
}

std::vector<std::string> splitCommandUser(const std::string& input) {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        if (token.empty()) continue;
        if (!token.empty() && token[0] == ':') {
            // Combining tokens starting from ':' until the end
            std::string combinedToken = token.substr(1);
            while (std::getline(iss, token, ' ')) {
                combinedToken += " " + token;
            }
            result.push_back(combinedToken);
            break;
        } else {
            result.push_back(token);
        }
    }
    return result;
}


void        logs(std::string type, std::string message) {
    if (type == "ERROR")
        std::cout << ERROR << "[ERROR]:\t    " << RESET << message << std::endl;
    else if (type == "RECEIVED")
        std::cout << RECEIVED <<"[RECEIVED]:\t" << RESET << message << std::endl;
    else
        std::cout << LOG << "[LOG]:\t        " << RESET << message << std::endl;
}

// std::string trimString(const std::string& str) {
//     std::string result;
//     bool spaceFound = false;
//     bool nonWhitespaceFound = false;

//     for (size_t i = 0; i < str.size(); ++i) {
//         char c = str[i];
//         if ((std::isspace(c))) {
//             if (!spaceFound && nonWhitespaceFound) {
//                 result += ' ';
//                 spaceFound = true;
//             }
//         } else {
//             result += c;
//             spaceFound = false;
//             nonWhitespaceFound = true;
//         }
//     }
//     if (!result.empty() && result[result.length() - 1] == ' ') {
//         result = trimString(result);
//     }
//     return result;
// }

std::string trimString(const std::string& input) {
    size_t start = input.find_first_not_of(" \n\t\v\f\r");
    size_t end = input.find_last_not_of(" \n\t\v\f\r");
    if (start == std::string::npos || end == std::string::npos) {
        return "";  // Empty or whitespace-only string
    }
    return input.substr(start, end - start + 1);
}

std::string intToString(int value) {
    char buffer[20]; // Assurez-vous que la taille du tampon est suffisamment grande

    // Utilisation de sprintf pour formater la chaîne
    sprintf(buffer, "%d", value);

    // Conversion du tampon en std::string
    return std::string(buffer);
}