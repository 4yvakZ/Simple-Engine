#include "pch.h"

#include "Game.h"

#include "MyGameObject.h"
#include "Transform.h"
#include "Player.h"

using namespace SimpleEngine;
using namespace DirectX::SimpleMath;


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
	auto player = Game::createGameObject<Player>();
	Transform transform;
	transform.setPosition(Vector3(0, 0, 5));
	player->setTransform(transform);

	game->run();
}