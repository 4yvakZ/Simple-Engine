#include "pch.h"
#include "RenderComponent.h"

#include "Game.h"
#include "RenderSystem.h"

#include "Material.h"
#include "Mesh.h"

SimpleEngine::RenderComponent::RenderComponent(std::shared_ptr<Material> material):
	mMaterial(material)
{
}

void SimpleEngine::RenderComponent::init()
{
	auto device = Game::getRenderSystem()->getDevice();

	mMaterial->init(device);
	/*
	///const buffer initialization
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstBufferData);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &constBufferData;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;

	render->Device->CreateBuffer(&constBufDesc, &constData, &constBuffer);

	///material const buffer initialization
	D3D11_BUFFER_DESC materialBufDesc = {};
	materialBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufDesc.MiscFlags = 0;
	materialBufDesc.StructureByteStride = 0;
	materialBufDesc.ByteWidth = sizeof(MaterialBufferData);

	D3D11_SUBRESOURCE_DATA materialData = {};
	materialData.pSysMem = &materialBufferData;
	materialData.SysMemPitch = 0;
	materialData.SysMemSlicePitch = 0;

	render->Device->CreateBuffer(&materialBufDesc, &materialData, &materialBuffer);
	*/
}

void SimpleEngine::RenderComponent::update()
{
}

void SimpleEngine::RenderComponent::initVertexandIndexBuffer(const std::vector<VertexData>& vertecis, const std::vector<uint32_t>& indecis)
{
	auto device = Game::getRenderSystem()->getDevice();
	///vertix buffer initialization
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = static_cast<UINT>(sizeof(VertexData) * std::size(vertecis));

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertecis.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufDesc, &vertexData, mVertexBuffer.GetAddressOf());

	///indexBuffer initialization
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = static_cast<UINT>(sizeof(int) * std::size(indecis));

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indecis.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, mIndexBuffer.GetAddressOf());
}
