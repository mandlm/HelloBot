#include "MessageHandler.h"

#include <sstream>

void MessageHandler::replyUserUnknown(const tgbot::types::Message &message, const tgbot::methods::Api &api)
{
	api.sendMessage(std::to_string(message.chat.id), "Sorry, I don't know who you are.");
}

void MessageHandler::handle(const tgbot::types::Message &message, const tgbot::methods::Api &api)
{
	if (message.from == nullptr || knownUsers.find(*message.from->username) == knownUsers.cend())
	{
		replyUserUnknown(message, api);

		return;
	}

	std::ostringstream logMessage;
	logMessage << message.from->firstName
		<< " (" << *message.from->username << "): " 
		<< (message.text != nullptr ? *message.text : "<no text>");
	api.getLogger().info(logMessage.str());

	std::ostringstream reply;
	reply << "You said";
	if (message.text != nullptr)
	{
		reply << ": '" << *message.text << "'";
	}
	else
	{
		reply << " illegible things";
	}

	api.sendMessage(std::to_string(message.chat.id), reply.str());
}

