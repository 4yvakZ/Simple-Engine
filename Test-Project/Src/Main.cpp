#include "pch.h"

#include "Game.h"

#include "MyGameObject.h"
#include "Transform.h"

using namespace SimpleEngine;



#if defined(DEBUG) || defined(_DEBUG)
#include <direct.h>

std::string get_current_dir() {
	char buff[FILENAME_MAX]; //create string buffer to hold path
	auto a = _getcwd(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

int main() {

	std::cout << get_current_dir() << std::endl;
#else
int main() {
#endif
	auto game = Game::createGameInstance<Game>();

	auto myGameObject = Game::createGameObject<MyGameObject>();

	game->run();
}