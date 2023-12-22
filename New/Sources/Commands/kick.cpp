#include "./../../Includes/Server.hpp"


bool commandKick(Server* server, User* user, std::vector<std::string> args)
{
    // POUR AUE TU CAPTE LA COMMAND CEST /kick #chennel username sauf toi mdrr
	for (int i = 0; i < (int)args.size(); i++)
		std::cout << "args[" << i << "] = " << "[" << args[i] << "]" << std::endl;

    if (args.size() < 2)
    {
        user->sendMessage("461", "Not enough parameters");
        return false;
    }
    std::string channelName = args[1].substr(1);
    std::string userName = args[2];
    Channel* channel = server->getChannelByName(channelName);

	if (channel == NULL)
    {
        user->sendMessage("442", "Wrong Channel");
        return false;
	}
    User* targetUser = channel->getUserByUsername(userName);
    if (!targetUser)
    {
        user->sendMessage("441", "They aren't on that channel");
        return false;
    }
    if (channel->getOperatorByUsername(user->getUsername()) == false)
    {
        user->sendMessage("482", "You're not channel operator");
        return false;
    }
    if (channel->removeUser(targetUser) == false)
    {
        logs(ERROR, "Error removing user from channel");
        return false;
    }
    else
    {
        std::string Kickmessage = "KICK " + channelName + " " + userName + " :" + user->getUsername() + " " + args[3];
        logs(LOG, Kickmessage);
        std::string message = ":" + user->getCompleteName() + " KICK #" + channelName + " " + userName + " :" + user->getUsername()  + "\r\n";
        send(targetUser->getSocket(), message.c_str(), message.size(), 0);
        user->sendMessage("KICK", channelName + " " + userName + " :" + user->getUsername() + " " + args[3]);
    }

    std::vector<User*> users = channel->getUsers();
    for (int i = 0; i < (int)users.size(); i++) 
    {
        if (users[i] != targetUser)
        {   
            std::string message = ":" + user->getCompleteName() + " KICK #" + channelName + " " + userName + " :" + user->getUsername()  + "\r\n";
            send(users[i]->getSocket(), message.c_str(), message.size(), 0);
        }
    }
    return true;
}
