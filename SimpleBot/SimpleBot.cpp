#include "tgbot/bot.h"

#include <boost/program_options.hpp>

#include <set>
#include <string>
#include <iostream>

static const std::set<std::string> knownUsers = { "mandlm" };

struct ProgramSettings
{
	std::string token;
};

ProgramSettings getProgramSettings(int argc, char **argv)
{
	namespace po = boost::program_options;
	po::options_description optionsDescription("Allowed options");
	optionsDescription.add_options()
		("help", "show this help message")
		("token,t", po::value<std::string>(), "Telegram bot token")
	;

	po::variables_map configuredOptions;
	po::store(po::command_line_parser(argc, argv).options(optionsDescription).run(), 
			configuredOptions);
	po::notify(configuredOptions);

	if (configuredOptions.find("help") != configuredOptions.cend()
			|| configuredOptions.find("token") == configuredOptions.cend())
	{
		std::cout << optionsDescription << std::endl;
		throw std::runtime_error("invalid options");
	}

	return ProgramSettings{ configuredOptions["token"].as<std::string>() };
}

std::string fullName(const tgbot::types::User &user)
{
	std::ostringstream fullName;
	fullName << user.firstName;

	if (user.lastName != nullptr)
	{
		fullName << " " << *user.lastName;
	}

	return fullName.str();
}

void replyUserUnknown(const tgbot::types::Message &message, const tgbot::methods::Api &api)
{
	api.sendMessage(std::to_string(message.chat.id), "Sorry, I don't know who you are.");
}

void handleMessage(const tgbot::types::Message message, const tgbot::methods::Api &api)
{
	if (message.from == nullptr || knownUsers.find(*message.from->username) == knownUsers.cend())
	{
		replyUserUnknown(message, api);

		return;
	}

	std::cout << fullName(*message.from) 
		<< " (" << *message.from->username << "): " 
		<< (message.text != nullptr ? *message.text : "<no text>") << std::endl;

	api.sendMessage(std::to_string(message.chat.id), "You said:");
	if (message.text != nullptr)
	{
		api.sendMessage(std::to_string(message.chat.id), *message.text);
	}
	else
	{
		api.sendMessage(std::to_string(message.chat.id), "illegible things");
	}
}

int main(int argc, char **argv)
{
	try
	{
		auto settings = getProgramSettings(argc, argv);

		std::cout << "Running bot... " << std::endl;
		
		tgbot::LongPollBot bot(settings.token, { tgbot::types::UpdateType::MESSAGE });

		std::cout << "connected as " << bot.getMe().firstName << std::endl;

		bot.callback(handleMessage);
		bot.start();
	}
	catch (std::runtime_error &e)
	{
		std::cout << "exiting." << std::endl;
	
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

