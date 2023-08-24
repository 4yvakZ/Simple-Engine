#include "pch.h"

#include "Game.h"

#include "MyGameObject.h"
#include "Sun.h"
#include "Transform.h"
#include "Player.h"
#include "ShaderStructs.h"
#include "DebugRenderer.h"

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
#endif


int main() {
#if defined(DEBUG) || defined(_DEBUG)
	std::cout << get_current_dir() << std::endl;
#endif
	auto game = Game::CreateGameInstance<Game>();

	auto debugRenderer = DebugRenderer::GetInstance();
	debugRenderer->DrawDebugGrid(2);

	

	auto player = Game::CreateGameObject<Player>();

	auto sun = Game::CreateGameObject<Sun>();

	auto myGameObject = Game::CreateGameObject<MyGameObject>();
	auto myGameObject1 = Game::CreateGameObject<MyGameObject>();

	Transform transform;
	transform.SetPosition(Vector3(0, 1, 5));
	player->SetTransform(transform);
	transform.SetPosition(Vector3(4, 0, 0));
	//transform.SetScale(Vector3(1, 1, 4));
	myGameObject1->SetTransform(transform);
	myGameObject1->SetRotationSpeed(1);

	transform.SetPosition(Vector3(2, 0, 2));
	debugRenderer->DrawDebugBox(transform, Vector3(2, 3, 1), Color(1,1,0), 2);
	transform.SetPosition(Vector3(2, 0, 0));
	debugRenderer->DrawDebugSphere(transform, 0.5);

	game->Run();
}