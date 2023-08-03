#include "pch.h"

#include "Game.h"

#include "MyGameObject.h"

using namespace SimpleEngine;


int main() {

	auto game = Game::createGameInstance<Game>();

	auto myGameObject = Game::createGameObject<MyGameObject>();

	game->run();
}