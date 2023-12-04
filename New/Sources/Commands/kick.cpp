#include "./../../Includes/Server.hpp"


bool commandKick(Server* server, User* user, std::vector<std::string> args)
{
	for (int i = 0; i < (int)args.size(); i++)
		std::cout << "args[" << i << "] = " << "[" << args[i] << "]" << std::endl;

	std::cout << "COMMAND KICK" << std::endl;
    if (args.size() < 2)
    {
        user->sendMessage("461", "Not enough parameters");
        return false;
    }
    std::string channelName = args[1].substr(1);
	std::cout << "channelName = " << channelName << std::endl;
    std::string userName = args[2];
    std::cout << "userName = " << userName << std::endl;
    Channel* channel = server->getChannelByName(channelName);
	std::cout << "channelName = " << channelName << std::endl;
	// Copilot you have to print me what is in channel

	if (channel == NULL)
    {
        user->sendMessage("442", "Wrong Channel");
        return false;
	}
    User* targetUser = channel->getUserByUsername(userName);
	// std::cout << "targetUser"  << channel->getUserByUsername(targetUser) << std::endl;
    if (!targetUser)
    {
        user->sendMessage("441", "They aren't on that channel");
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
    user->sendMessage("KICK", channelName + " " + userName + " :" + user->getUsername() + " " + args[3]);
}


std::vector<User*> users = channel->getUsers();
std::cout << "users.size() = " << users.size() << std::endl;
for (int i = 0; i < (int)users.size(); i++)
{
	std::cout << "users[" << i << "] = " << "[" << users[i]->getUsername() << "]" << std::endl;
    if (users[i] != targetUser)
    {
        users[i]->sendMessage("KICK", channelName + " " + userName + " :" + user->getUsername() + " " + args[3]);
    }
}
return true;
}
