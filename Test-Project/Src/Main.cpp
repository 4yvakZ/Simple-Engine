#include "pch.h"

#include "Game.h"

#include "MyGameObject.h"
#include "Sun.h"
#include "Transform.h"
#include "Player.h"
#include "ShaderStructs.h"
#include "DebugRenderer.h"
#include "Lamp.h"

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
	Transform transform;
	transform.SetPosition(Vector3(0, 1, 5));
	player->SetTransform(transform);
	
	auto sun = Game::CreateGameObject<Sun>();
	//auto sun0 = Game::CreateGameObject<Sun>();
	//sun0->SetLightDirection(Vector3(1, -1.5, -1));

	auto myGameObject = Game::CreateGameObject<MyGameObject>();
	transform.SetPosition(Vector3(2, 1, 0));
	myGameObject->SetTransform(transform);

	auto myGameObject1 = Game::CreateGameObject<MyGameObject>();
	transform.SetPosition(Vector3(-2, 2, 2));
	myGameObject1->SetTransform(transform);
	myGameObject1->SetRotationSpeed(1);

	auto lamp = Game::CreateGameObject<Lamp>();
	transform.SetPosition(Vector3(-1, 1, -3));
	lamp->SetTransform(transform);

	auto plane = Game::CreateGameObject<MyGameObject>("../assets/plane.fbx");
	transform.SetPosition(Vector3(0, -1, 0));
	transform.SetRotation(Quaternion::FromToRotation(Vector3::UnitZ, Vector3::UnitY));
	transform.SetScale(Vector3(10, 10, 1));
	plane->SetTransform(transform);

	transform.SetPosition(Vector3(2, 0, 2));
	debugRenderer->DrawDebugBox(transform, Vector3(2, 3, 1), Color(1,1,0), false, 2);

	game->Run();
}