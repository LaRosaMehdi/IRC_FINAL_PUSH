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
std::cout<< "commandInvite" << std::endl;
		std::cout << std::endl;

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
		std::cout << "Ceci EST le test" << std::endl;
		// std::cout << channel->getOperatorByUsername(user->getUsername())->getUsername() << std::endl;
		if (user == channel->getOperatorByUsername(user->getUsername())){
    // :inviterNick!inviterUser@inviter.host.com 341 yourNick #channel

			std::string message = ":" + user->getCompleteName() + " 341 " + targetUser->getNickname() + " #" + channel->getName() + "\r\n";
			send(targetUser->getSocket(), message.c_str(), message.size(), 0);
			channel->addInvited(user, targetUser);
			for (int i = 0; i < (int)users.size(); i++)
			{
				if (users[i] != targetUser)
				{
				}
			}
			logs(LOG, message);
			}
		else 
		{
			user->sendMessage("482", "-!- Irssi: You're not channel operator");
			return false;
		}
		// channel->addUserToInviteList(targetUser);
		
		//for (int i = 0; i < (int)channel->getInvitesList().size(); i++)
		//{
		//	std::cout << "TEMA LA LISTE D'INVITE [" << i << "] = " << channel->getInvitesList()[i]->getNickname() << std::endl;
		//}
		
		 	
	}
	return true;
}


