cmake_minimum_required(VERSION 3.5)

find_package(Boost 1.60.0 COMPONENTS program_options REQUIRED)

project(SimpleBot LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
	SimpleBot.cpp
	MessageHandler.cpp
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE 
		tgbot
		Boost::program_options
)

target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_17
)

