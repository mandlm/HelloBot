#include "MessageHandler.h"
#include "tgbot/bot.h"

#include <boost/program_options.hpp>

#include <set>
#include <string>
#include <iostream>

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

int main(int argc, char **argv)
{
	try
	{
		auto settings = getProgramSettings(argc, argv);

		std::cout << "Running bot... " << std::endl;
		
		tgbot::LongPollBot bot(settings.token, { tgbot::types::UpdateType::MESSAGE });

		std::cout << "connected as " << bot.getMe().firstName << std::endl;

		MessageHandler messageHandler;
		bot.callback([&messageHandler]
			(const tgbot::types::Message message, const tgbot::methods::Api &api)
			{
				messageHandler.handle(message, api);
			});

		bot.start();
	}
	catch (std::runtime_error &e)
	{
		std::cout << "exiting." << std::endl;
	
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

