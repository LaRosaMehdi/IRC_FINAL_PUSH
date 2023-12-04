#include "./../../Includes/Server.hpp"


bool commandKick(Server* server, User* user, std::vector<std::string> args)
{
	for (int i = 0; i < (int)args.size(); i++)
		std::cout << "args[" << i << "] = " << "[" << args[i] << "]" << std::endl;

	std::cout << "COMMAND KICK" << std::endl;
    if (args.size() < 2)
    {
        server->sendMessage("461", "Not enough parameters", user);
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
        server->sendMessage("442", "Wrong Channel", user);
        return false;
	}
    User* targetUser = channel->getUserByUsername(userName);
	// std::cout << "targetUser"  << channel->getUserByUsername(targetUser) << std::endl;
    if (!targetUser)
    {
        server->sendMessage("441", "They aren't on that channel", user);
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
    server->sendMessage("KICK", channelName + " " + userName + " :" + user->getUsername() + " " + args[3], targetUser);
}


std::vector<User*> users = channel->getUsers();
std::cout << "users.size() = " << users.size() << std::endl;
for (int i = 0; i < (int)users.size(); i++)
{
	std::cout << "users[" << i << "] = " << "[" << users[i]->getUsername() << "]" << std::endl;
    if (users[i] != targetUser)
    {
        server->sendMessage("KICK", channelName + " " + userName + " :" + user->getUsername() + " " + args[3], users[i]);
    }
}
return true;
}
