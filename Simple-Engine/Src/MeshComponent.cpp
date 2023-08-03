#include "pch.h"
#include "MeshComponent.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Mesh.h"

SimpleEngine::MeshComponent::MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh):
	RenderComponent(material),
	mMesh(mesh)
{
}

void SimpleEngine::MeshComponent::draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
}

void SimpleEngine::MeshComponent::init()
{
	RenderComponent::init();

	initVertexandIndexBuffer(mMesh->mVertecis, mMesh->mIndecis);
}
