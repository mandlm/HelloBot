#pragma once

#include "tgbot/bot.h"

#include <string>
#include <set>

class MessageHandler
{
	private:
		const std::set<std::string> knownUsers = { "mandlm" };

	private:
		void replyUserUnknown(const tgbot::types::Message &message, const tgbot::methods::Api &api);

	public:
		void handle(const tgbot::types::Message &message, const tgbot::methods::Api &api);
};

