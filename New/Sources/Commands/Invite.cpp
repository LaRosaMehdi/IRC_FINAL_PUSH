#include "./../../Includes/Server.hpp"
#include "./../../Includes/Channel.hpp"

bool    commandInvite(Server *server, User *user, std::vector<std::string> args)
{
	if (args.size() < 3)
	{
		user->sendMessage("461", "Not enough parameters");
		return false;
	}

	std::string channelName = args[2].substr(1);
	std::string userName = args[1];
	Channel* channel = server->getChannelByName(channelName);

	if (channel == NULL)
	{
		user->sendMessage("442", "Wrong Channel");
		return false;
	}
	User* targetUser = server->getUserByUsername(userName);
	if (!targetUser)
	{
		user->sendMessage("441", "They aren't on that channel");
		return false;
	}
	else
	{	
		std::vector<User*> users = channel->getUsers();
		std::string message = ":" + user->getCompleteName() + " INVITE #" + targetUser->getNickname() + " " + channel->getName() + "\r\n";
		send(targetUser->getSocket(), message.c_str(), message.size(), 0);
		for (int i = 0; i < (int)users.size() + 1; i++)
		{
			if (users[i] != targetUser)
			{
			}
		}
		logs(LOG, message);
		channel->addUserToInviteList(targetUser);
		
		//for (int i = 0; i < (int)channel->getInvitesList().size(); i++)
		//{
		//	std::cout << "TEMA LA LISTE D'INVITE [" << i << "] = " << channel->getInvitesList()[i]->getNickname() << std::endl;
		//}
		
		 	
	}
	return true;
}


