#include "pch.h"
#include "DebugRenderer.h"
#include "Game.h"
#include "RenderSystem.h"
#include "AssetManager.h"
#include "Material.h"

constexpr int kGridSize = 100;
constexpr int kSphereStacks = 12;

using namespace DirectX::SimpleMath;


void SimpleEngine::DebugRenderer::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	UINT strides[] = { sizeof(VertexData) };
	UINT offsets[] = { 0 };

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);

	for (auto it = mDebugShapes.begin(); it < mDebugShapes.end(); it++)
	{
		UpdateObjectConstBuffer(context, it->mTransform);
		it->mMaterial->Bind(context);

		context->VSSetConstantBuffers(1, 1, mObjectConstBuffer.GetAddressOf());

		context->DrawIndexed(mIndexCounts[it->mType], mStartIndexLocations[it->mType], 0);
	}
}

void SimpleEngine::DebugRenderer::Update(float deltaTime)
{
	for (auto it = mDebugShapes.begin(); it < mDebugShapes.end();)
	{
		if (!it->mIsImmortal)
		{
			it->mLifeTime -= deltaTime;
			if (it->mLifeTime < 0)
			{
				it = mDebugShapes.erase(it);
				continue;
			}
		}
		it++;
	}
}

void SimpleEngine::DebugRenderer::Init(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	InitObjectConstBuffer(device);

	std::vector<uint32_t> indecis;
	std::vector<VertexData> vertecis;

	InitGridVertecis(vertecis, indecis);
	InitVectorVertecis(vertecis, indecis);
	InitBoxVerticis(vertecis, indecis);
	InitSphereVerticis(vertecis, indecis);
	
	InitVertexBuffer(device, vertecis);
	InitIndexBuffer(device, indecis);
}

void SimpleEngine::DebugRenderer::InitGridVertecis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis)
{
	auto firstPointIndex = vertecis.size();
	mStartIndexLocations[DebugShapeType::Grid] = static_cast<int>(indecis.size());
	const int nPoints = kGridSize * 2 + 1;
	float offset = static_cast<float>(- (nPoints / 2));
	
	for (int i = 0; i < nPoints; i++)
	{
		for (int j = 0; j < nPoints; j++)
		{
			switch (i)
			{
			case 0:
			case nPoints - 1:
				vertecis.emplace_back(Vector3(i + offset, 0, j + offset));
				break;

			default:
				switch (j)
				{
				case 0:
				case nPoints - 1:
					vertecis.emplace_back(Vector3(i + offset, 0, j + offset));
					break;

				default:
					break;
				}
				break;
			}
		}
	}

	vertecis.emplace_back(Vector3::Zero); 
	vertecis.emplace_back(Vector3::UnitX);
	vertecis.emplace_back(Vector3::UnitZ);

	for (int i = 0; i < nPoints; i++)
	{
		if (i == nPoints / 2) 
		{
			indecis.push_back(firstPointIndex + i);
			indecis.push_back(firstPointIndex + 4 * (nPoints - 1));
			indecis.push_back(firstPointIndex + 4 * (nPoints - 1) + 1);
			indecis.push_back(firstPointIndex + i + nPoints + (nPoints - 2) * 2);
			continue;
		}
		indecis.push_back(firstPointIndex + i);
		indecis.push_back(firstPointIndex + i + nPoints + (nPoints - 2) * 2);
	}

	indecis.push_back(firstPointIndex);
	indecis.push_back(firstPointIndex + nPoints - 1);

	for (int i = 0; i < nPoints - 2; i++)
	{
		if (i == (nPoints - 2) / 2)
		{
			indecis.push_back(firstPointIndex + 2 * i + nPoints);
			indecis.push_back(firstPointIndex + 4 * (nPoints - 1));
			indecis.push_back(firstPointIndex + 4 * (nPoints - 1) + 2);
			indecis.push_back(firstPointIndex + 2 * i + nPoints + 1);
			continue;
		}
		indecis.push_back(firstPointIndex + 2 * i + nPoints);
		indecis.push_back(firstPointIndex + 2 * i + nPoints + 1);
	}

	indecis.push_back(firstPointIndex + 4 * (nPoints - 1) - 1);
	indecis.push_back(firstPointIndex + 3 * (nPoints - 1) - 1);

	mIndexCounts[DebugShapeType::Grid] = indecis.size() - mStartIndexLocations[DebugShapeType::Grid] + 1;
}

void SimpleEngine::DebugRenderer::InitVectorVertecis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis)
{
	auto firstPointIndex = vertecis.size();
	vertecis.emplace_back(Vector3(0.0, 0.0, 0.0));
	vertecis.emplace_back(Vector3::UnitX);

	mStartIndexLocations[DebugShapeType::Vector] = static_cast<int>(indecis.size());
	mIndexCounts[DebugShapeType::Vector] = 2;
	indecis.push_back(firstPointIndex);
	indecis.push_back(firstPointIndex + 1);
}

void SimpleEngine::DebugRenderer::InitBoxVerticis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis)
{
	auto firstPointIndex = vertecis.size();

	for (int i = 0; i < 8; i++) 
	{
		vertecis.emplace_back(Vector3((i & 1) - 0.5, ((i >> 1) & 1) - 0.5, (i >> 2) - 0.5));
	}

	mStartIndexLocations[DebugShapeType::Box] = static_cast<int>(indecis.size());
	mIndexCounts[DebugShapeType::Box] = 24;
	for (int i = 0; i < 4; i++)
	{
		indecis.push_back(firstPointIndex + 2 * i);
		indecis.push_back(firstPointIndex + 2 * i + 1);

		indecis.push_back(firstPointIndex + i);
		indecis.push_back(firstPointIndex + i + 4);

		if (i < 2)
		{
			indecis.push_back(firstPointIndex + i);
			indecis.push_back(firstPointIndex + i + 2);
		}
		else
		{
			indecis.push_back(firstPointIndex + i + 2);
			indecis.push_back(firstPointIndex + i + 4);
		}
	}

}

void SimpleEngine::DebugRenderer::InitSphereVerticis(std::vector<SimpleEngine::VertexData>& vertecis, std::vector<uint32_t>& indecis)
{
	// add top vertex
	auto firstPointIndex = vertecis.size();
	vertecis.emplace_back(Vector3(0, 1, 0));
	// generate vertices per stack / slice
	for (int i = 0; i < kSphereStacks - 1; i++)
	{
		auto phi = DirectX::XM_PI * double(i + 1) / double(kSphereStacks);
		for (int j = 0; j < kSphereStacks; j++)
		{
			auto theta = 2.0 * DirectX::XM_PI * double(j) / double(kSphereStacks);
			auto x = std::sin(phi) * std::cos(theta);
			auto y = std::cos(phi);
			auto z = std::sin(phi) * std::sin(theta);
			vertecis.emplace_back(Vector3(x, y, z));
		}
	}

	// add bottom vertex
	auto lastPointIndex = vertecis.size();
	vertecis.emplace_back(Vector3(0, -1, 0));

	mStartIndexLocations[DebugShapeType::Sphere] = static_cast<int>(indecis.size());
	// add top / bottom triangles
	for (int i = 0; i < kSphereStacks; ++i)
	{
		auto i0 = i + 1 + firstPointIndex;
		auto i1 = (i + 1) % kSphereStacks + 1 + firstPointIndex;
		indecis.push_back(firstPointIndex);
		indecis.push_back(i1);
		indecis.push_back(i0);
		indecis.push_back(firstPointIndex);
		indecis.push_back(i1);
		indecis.push_back(i0);
		i0 = i + kSphereStacks * (kSphereStacks - 2) + 1 + firstPointIndex;
		i1 = (i + 1) % kSphereStacks + kSphereStacks * (kSphereStacks - 2) + 1 + firstPointIndex;
		indecis.push_back(lastPointIndex);
		indecis.push_back(i0);
		indecis.push_back(i1);
		indecis.push_back(lastPointIndex);
		indecis.push_back(i0);
		indecis.push_back(i1);
	}

	// add quads per stack / slice
	for (int j = 0; j < kSphereStacks - 2; j++)
	{
		auto j0 = j * kSphereStacks + 1;
		auto j1 = (j + 1) * kSphereStacks + 1;
		for (int i = 0; i < kSphereStacks; i++)
		{
			auto i0 = j0 + i + firstPointIndex;
			auto i1 = j0 + (i + 1) % kSphereStacks + firstPointIndex;
			auto i2 = j1 + (i + 1) % kSphereStacks + firstPointIndex;
			auto i3 = j1 + i + firstPointIndex;
			indecis.push_back(i0);
			indecis.push_back(i1); 
			indecis.push_back(i1);
			indecis.push_back(i2);
			indecis.push_back(i2);
			indecis.push_back(i3);
			indecis.push_back(i3);
			indecis.push_back(i0);
		}
	}

	mIndexCounts[DebugShapeType::Sphere] = indecis.size() - mStartIndexLocations[DebugShapeType::Sphere];
}

void SimpleEngine::DebugRenderer::DrawDebugSphere(const Transform& centerTransform = Transform(), float radius, DirectX::SimpleMath::Color color, float lifeTime)
{
	Transform transform = centerTransform;
	transform.SetScale(Vector3(radius, radius, radius));

	DebugShape sphere;
	sphere.mTransform = transform;
	sphere.mType = DebugShapeType::Sphere;
	if (lifeTime == -1)
	{
		sphere.mIsImmortal = true;
	}
	else
	{
		sphere.mIsImmortal = false;
		sphere.mLifeTime = lifeTime;
	}
	sphere.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	sphere.mMaterial->SetAlbedo(color);

	mDebugShapes.push_back(sphere);
}

void SimpleEngine::DebugRenderer::DrawDebugBox(const Transform& centerTransform = Transform(), DirectX::SimpleMath::Vector3 size, DirectX::SimpleMath::Color color, float lifeTime)
{
	Transform transform = centerTransform;
	transform.SetScale(size);

	DebugShape box;
	box.mTransform = transform;
	box.mType = DebugShapeType::Box;
	if (lifeTime == -1)
	{
		box.mIsImmortal = true;
	}
	else
	{
		box.mIsImmortal = false;
		box.mLifeTime = lifeTime;
	}
	box.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	box.mMaterial->SetAlbedo(color);

	mDebugShapes.push_back(box);
}

void SimpleEngine::DebugRenderer::DrawDebugVector(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Color color, float lifeTime)
{
	Vector3 vector = end - start;

	Transform transform;
	transform.SetPosition(start);

	float length = vector.Length();
	transform.SetScale(Vector3(length, length, length));

	vector.Normalize();
	transform.SetRotation(Quaternion::FromToRotation(Vector3::UnitX, vector));

	DebugShape vectorShape;
	vectorShape.mTransform = transform;
	vectorShape.mType = DebugShapeType::Vector;
	if (lifeTime == -1) 
	{
		vectorShape.mIsImmortal = true;
	}
	else
	{
		vectorShape.mIsImmortal = false;
		vectorShape.mLifeTime = lifeTime;
	}
	vectorShape.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	vectorShape.mMaterial->SetAlbedo(color);

	mDebugShapes.push_back(vectorShape);
}

void SimpleEngine::DebugRenderer::DrawDebugGrid(float cellSize)
{
	Transform transform;
	transform.SetScale(Vector3(cellSize, cellSize, cellSize));

	DebugShape grid;
	grid.mTransform = transform;
	grid.mType = DebugShapeType::Grid;
	grid.mIsImmortal = true;
	grid.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	grid.mMaterial->SetAlbedo(Color(1.0, 1.0, 1.0, 1.0) * 0.5);

	mDebugShapes.push_back(grid);

	DrawDebugVector(Vector3::Zero, Vector3::UnitX * cellSize, Color(1.0, 0.0, 0.0, 1.0) * 0.5);
	DrawDebugVector(Vector3::Zero, Vector3::UnitY * cellSize, Color(0.0, 1.0, 0.0, 1.0) * 0.5);
	DrawDebugVector(Vector3::Zero, Vector3::UnitZ * cellSize, Color(0.0, 0.0, 1.0, 1.0) * 0.5);
}

std::shared_ptr<SimpleEngine::DebugRenderer> SimpleEngine::DebugRenderer::GetInstance()
{
	if (!sInstance)
	{
		sInstance = std::shared_ptr<DebugRenderer>(new DebugRenderer());
	}
	return sInstance;
}

SimpleEngine::DebugRenderer::DebugRenderer()
{
}

void SimpleEngine::DebugRenderer::InitObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(mObjectConstBufferData);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &mObjectConstBufferData;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;

	device->CreateBuffer(&constBufDesc, &constData, mObjectConstBuffer.GetAddressOf());

}

void SimpleEngine::DebugRenderer::UpdateObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, const Transform& transform)
{
	mObjectConstBufferData.mWorld = transform.GetWorld();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(mObjectConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &mObjectConstBufferData, sizeof(ObjectConstBufferData));

	context->Unmap(mObjectConstBuffer.Get(), 0);
}

void SimpleEngine::DebugRenderer::InitVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<VertexData>& vertecis)
{
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
}

void SimpleEngine::DebugRenderer::InitIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indecis)
{
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
