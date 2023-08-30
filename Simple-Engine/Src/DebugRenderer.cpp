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
	UINT strides[] = { sizeof(Vertex) };
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

	while (!mDebugShapesQueue.empty()) 
	{
		auto shape = mDebugShapesQueue.front();
		shape.mMaterial->Init();
		shape.mMaterial->Bind(context);
		UpdateObjectConstBuffer(context, shape.mTransform);

		context->VSSetConstantBuffers(1, 1, mObjectConstBuffer.GetAddressOf());

		context->DrawIndexed(mIndexCounts[shape.mType], mStartIndexLocations[shape.mType], 0);
		mDebugShapesQueue.pop();
	}
}

void SimpleEngine::DebugRenderer::Update(float deltaTime)
{
	for (auto it = mDebugShapes.begin(); it < mDebugShapes.end();)
	{
		if (!it->mIsPermanent)
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

	std::vector<uint32_t> indices;
	std::vector<Vertex> vertices;

	InitGridvertices(vertices, indices);
	InitVectorvertices(vertices, indices);
	InitBoxVerticis(vertices, indices);
	InitSphereVerticis(vertices, indices);
	
	InitVertexBuffer(device, vertices);
	InitIndexBuffer(device, indices);
}

void SimpleEngine::DebugRenderer::InitGridvertices(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices)
{
	auto firstPointIndex = vertices.size();
	mStartIndexLocations[DebugShapeType::Grid] = static_cast<int>(indices.size());
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
				vertices.emplace_back(Vector3(i + offset, 0, j + offset));
				break;

			default:
				switch (j)
				{
				case 0:
				case nPoints - 1:
					vertices.emplace_back(Vector3(i + offset, 0, j + offset));
					break;

				default:
					break;
				}
				break;
			}
		}
	}

	vertices.emplace_back(Vector3::Zero); 
	vertices.emplace_back(Vector3::UnitX);
	vertices.emplace_back(Vector3::UnitZ);

	for (int i = 0; i < nPoints; i++)
	{
		if (i == nPoints / 2) 
		{
			indices.push_back(firstPointIndex + i);
			indices.push_back(firstPointIndex + 4 * (nPoints - 1));
			indices.push_back(firstPointIndex + 4 * (nPoints - 1) + 1);
			indices.push_back(firstPointIndex + i + nPoints + (nPoints - 2) * 2);
			continue;
		}
		indices.push_back(firstPointIndex + i);
		indices.push_back(firstPointIndex + i + nPoints + (nPoints - 2) * 2);
	}

	indices.push_back(firstPointIndex);
	indices.push_back(firstPointIndex + nPoints - 1);

	for (int i = 0; i < nPoints - 2; i++)
	{
		if (i == (nPoints - 2) / 2)
		{
			indices.push_back(firstPointIndex + 2 * i + nPoints);
			indices.push_back(firstPointIndex + 4 * (nPoints - 1));
			indices.push_back(firstPointIndex + 4 * (nPoints - 1) + 2);
			indices.push_back(firstPointIndex + 2 * i + nPoints + 1);
			continue;
		}
		indices.push_back(firstPointIndex + 2 * i + nPoints);
		indices.push_back(firstPointIndex + 2 * i + nPoints + 1);
	}

	indices.push_back(firstPointIndex + 4 * (nPoints - 1) - 1);
	indices.push_back(firstPointIndex + 3 * (nPoints - 1) - 1);

	mIndexCounts[DebugShapeType::Grid] = indices.size() - mStartIndexLocations[DebugShapeType::Grid];
}

void SimpleEngine::DebugRenderer::InitVectorvertices(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices)
{
	auto firstPointIndex = vertices.size();
	vertices.emplace_back(Vector3(0.0, 0.0, 0.0));
	vertices.emplace_back(Vector3::UnitX);

	mStartIndexLocations[DebugShapeType::Vector] = static_cast<int>(indices.size());
	mIndexCounts[DebugShapeType::Vector] = 2;
	indices.push_back(firstPointIndex);
	indices.push_back(firstPointIndex + 1);
}

void SimpleEngine::DebugRenderer::InitBoxVerticis(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices)
{
	auto firstPointIndex = vertices.size();

	for (int i = 0; i < 8; i++) 
	{
		vertices.emplace_back(Vector3((i & 1) - 0.5, ((i >> 1) & 1) - 0.5, (i >> 2) - 0.5));
	}

	mStartIndexLocations[DebugShapeType::Box] = static_cast<int>(indices.size());
	mIndexCounts[DebugShapeType::Box] = 24;
	for (int i = 0; i < 4; i++)
	{
		indices.push_back(firstPointIndex + 2 * i);
		indices.push_back(firstPointIndex + 2 * i + 1);

		indices.push_back(firstPointIndex + i);
		indices.push_back(firstPointIndex + i + 4);

		if (i < 2)
		{
			indices.push_back(firstPointIndex + i);
			indices.push_back(firstPointIndex + i + 2);
		}
		else
		{
			indices.push_back(firstPointIndex + i + 2);
			indices.push_back(firstPointIndex + i + 4);
		}
	}

}

void SimpleEngine::DebugRenderer::InitSphereVerticis(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices)
{
	// add top vertex
	auto firstPointIndex = vertices.size();
	vertices.emplace_back(Vector3(0, 1, 0));
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
			vertices.emplace_back(Vector3(x, y, z));
		}
	}

	// add bottom vertex
	auto lastPointIndex = vertices.size();
	vertices.emplace_back(Vector3(0, -1, 0));

	mStartIndexLocations[DebugShapeType::Sphere] = static_cast<int>(indices.size());
	// add top / bottom triangles
	for (int i = 0; i < kSphereStacks; ++i)
	{
		auto i0 = i + 1 + firstPointIndex;
		auto i1 = (i + 1) % kSphereStacks + 1 + firstPointIndex;
		indices.push_back(firstPointIndex);
		indices.push_back(i1);
		indices.push_back(i0);
		indices.push_back(firstPointIndex);
		indices.push_back(i1);
		indices.push_back(i0);
		i0 = i + kSphereStacks * (kSphereStacks - 2) + 1 + firstPointIndex;
		i1 = (i + 1) % kSphereStacks + kSphereStacks * (kSphereStacks - 2) + 1 + firstPointIndex;
		indices.push_back(lastPointIndex);
		indices.push_back(i0);
		indices.push_back(i1);
		indices.push_back(lastPointIndex);
		indices.push_back(i0);
		indices.push_back(i1);
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
			indices.push_back(i0);
			indices.push_back(i1); 
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i2);
			indices.push_back(i3);
			indices.push_back(i3);
			indices.push_back(i0);
		}
	}

	mIndexCounts[DebugShapeType::Sphere] = indices.size() - mStartIndexLocations[DebugShapeType::Sphere];
}

void SimpleEngine::DebugRenderer::DrawDebugSphere(const Transform& centerTransform = Transform(), float radius, DirectX::SimpleMath::Color color, bool isPermanent, float lifeTime)
{
	Transform transform = centerTransform;
	transform.SetScale(Vector3(radius, radius, radius));

	DebugShape sphere;
	sphere.mTransform = transform;
	sphere.mType = DebugShapeType::Sphere;
	sphere.mIsPermanent = isPermanent;
	sphere.mLifeTime = lifeTime;
	sphere.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	sphere.mMaterial->SetAlbedo(color);

	if (lifeTime == -1 && !isPermanent)
	{
		mDebugShapesQueue.push(sphere);
	}
	else
	{
		mDebugShapes.push_back(sphere);
	}
}

void SimpleEngine::DebugRenderer::DrawDebugBox(const Transform& centerTransform = Transform(), DirectX::SimpleMath::Vector3 size, DirectX::SimpleMath::Color color, bool isPermanent, float lifeTime)
{
	Transform transform = centerTransform;
	transform.SetScale(size);

	DebugShape box;
	box.mTransform = transform;
	box.mType = DebugShapeType::Box;
	box.mIsPermanent = isPermanent;
	box.mLifeTime = lifeTime;
	box.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	box.mMaterial->SetAlbedo(color);

	if (lifeTime == -1 && !isPermanent)
	{
		mDebugShapesQueue.push(box);
	}
	else
	{
		mDebugShapes.push_back(box);
	}
}

void SimpleEngine::DebugRenderer::DrawDebugVector(DirectX::SimpleMath::Vector3 start, DirectX::SimpleMath::Vector3 end, DirectX::SimpleMath::Color color, bool isPermanent, float lifeTime)
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
	vectorShape.mIsPermanent = isPermanent;
	vectorShape.mLifeTime = lifeTime;
	vectorShape.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	vectorShape.mMaterial->SetAlbedo(color);

	if (lifeTime == -1 && !isPermanent) 
	{
		mDebugShapesQueue.push(vectorShape);
	}
	else
	{
		mDebugShapes.push_back(vectorShape);
	}
}

void SimpleEngine::DebugRenderer::DrawDebugGrid(float cellSize)
{
	Transform transform;
	transform.SetScale(Vector3(cellSize, cellSize, cellSize));

	DebugShape grid;
	grid.mTransform = transform;
	grid.mType = DebugShapeType::Grid;
	grid.mIsPermanent = true;
	grid.mMaterial = AssetManager::GetInstance()->CreateMaterial(MaterialType::Debug);
	grid.mMaterial->SetAlbedo(Color(1.0, 1.0, 1.0, 1.0) * 0.5);

	mDebugShapes.push_back(grid);

	DrawDebugVector(Vector3::Zero, Vector3::UnitX * cellSize, Color(1.0, 0.0, 0.0, 1.0) * 0.5, true);
	DrawDebugVector(Vector3::Zero, Vector3::UnitY * cellSize, Color(0.0, 1.0, 0.0, 1.0) * 0.5, true);
	DrawDebugVector(Vector3::Zero, Vector3::UnitZ * cellSize, Color(0.0, 0.0, 1.0, 1.0) * 0.5, true);
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

void SimpleEngine::DebugRenderer::InitVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<Vertex>& vertices)
{
	///vertix buffer initialization
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * std::size(vertices));

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufDesc, &vertexData, mVertexBuffer.GetAddressOf());
}

void SimpleEngine::DebugRenderer::InitIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indices)
{
	///indexBuffer initialization
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = static_cast<UINT>(sizeof(int) * std::size(indices));

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, mIndexBuffer.GetAddressOf());
}
