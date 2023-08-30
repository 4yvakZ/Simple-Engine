#include "pch.h"
#include "PointLightComponent.h"
#include "AssetManager.h"
#include "Game.h"
#include "RenderSystem.h"
#include "Material.h"
#include "CameraComponent.h"
#include "Utils.h"

using namespace DirectX::SimpleMath;
constexpr Vector4 kDefaultLightIntensity = Vector4(1, 1, 1, 0);
constexpr int kNumberGeosphereSubdivision = 3;

const std::string kQuadVSName = "../shaders/DefaultAlignedQuadVS.hlsl";

SimpleEngine::PointLightComponent::PointLightComponent():
	//TODO DefaultPointLightMaterial
	mMeshMaterial(AssetManager::GetInstance()->GetDefaultPointLightMeshMaterial()),
	mQuadMaterial(AssetManager::GetInstance()->GetDefaultPointLightQuadMaterial())
{
	mLightConstBufferData.mIntensity = kDefaultLightIntensity;
}

void SimpleEngine::PointLightComponent::Update()
{

}

void SimpleEngine::PointLightComponent::Init()
{
	auto device = Game::GetRenderSystem()->getDevice();

	InitDepthStencilStates(device);

	InitLightConstBuffer(device);
	InitObjectConstBuffer(device);

	std::vector<uint32_t> indices;
	std::vector<Vertex> vertices;

	CreateGeospherevertices(vertices, indices);

	mNumberOfIndices = static_cast<uint32_t>(indices.size());

	InitVertexBuffer(device, vertices);
	InitIndexBuffer(device, indices);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rastDesc, mFrontCullingRastState.GetAddressOf());
}

void SimpleEngine::PointLightComponent::OnConstructed()
{
	Component::OnConstructed();

	Game::GetRenderSystem()->AddPointLightComponent(std::dynamic_pointer_cast<PointLightComponent>(shared_from_this()));
}

void SimpleEngine::PointLightComponent::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->PSSetConstantBuffers(2, 1, mLightConstBuffer.GetAddressOf());
}

void SimpleEngine::PointLightComponent::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	UpdateLightConstBuffer(context);
	UpdateObjectConstBuffer(context);

	auto camera = Game::GetInstance()->GetActiveCameraComponent();
	auto pos = GetWorldTransform().GetPosition();

	float inFrustrumZ = Vector4::Transform(ToVector4(pos, 1.0), camera->GetViewProjection()).z;

	//std::cout << inFrustrumZ << "\n";

	float nearPlane = camera->GetNearPlane();
	float farPlane = camera->GetFarPlane();

	Bind(context);


	if (inFrustrumZ - mLightRadius - nearPlane < -1e-5)
	{
		//Draw Full Screen Quad
		mQuadMaterial->Bind(context);

		context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		context->OMGetDepthStencilState(mDepthStencilStateOff.GetAddressOf(), 0);
		context->Draw(4, 0);

		//std::cout << "Quad\n";
	}
	else
	{
		//Draw Sphere
		mMeshMaterial->Bind(context);
		
		if (inFrustrumZ + mLightRadius - farPlane < -1e-5)
		{
			//std::cout << "In frusrum mesh\n";
			context->RSSetState(mFrontCullingRastState.Get());
			context->OMSetDepthStencilState(mDepthStencilStateGreater.Get(), 0);
		}
		else
		{
			//Intersects far plane
			//std::cout << "Intersects far plane mesh\n";
			context->OMSetDepthStencilState(mDepthStencilStateLess.Get(), 0);
		}
		
		UINT strides[] = { sizeof(Vertex) };
		UINT offsets[] = { 0 };

		context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);

		context->VSSetConstantBuffers(1, 1, mObjectConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(1, 1, mObjectConstBuffer.GetAddressOf());

		context->DrawIndexed(mNumberOfIndices, 0, 0);
	}
}

DirectX::SimpleMath::Vector4 SimpleEngine::PointLightComponent::GetLightIntensity() const
{
	return mLightConstBufferData.mIntensity;
}

void SimpleEngine::PointLightComponent::SetLightIntensity(DirectX::SimpleMath::Vector4 lightIntensity)
{
	mLightConstBufferData.mIntensity = lightIntensity;
	//auto mean
	//mLightRadius = sqrt
}

void SimpleEngine::PointLightComponent::InitLightConstBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(mLightConstBufferData);

	D3D11_SUBRESOURCE_DATA constData = {};
	constData.pSysMem = &mLightConstBufferData;
	constData.SysMemPitch = 0;
	constData.SysMemSlicePitch = 0;
	device->CreateBuffer(&constBufDesc, &constData, mLightConstBuffer.GetAddressOf());
}

void SimpleEngine::PointLightComponent::UpdateLightConstBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	mLightConstBufferData.mPosition = ToVector4(GetWorldTransform().GetPosition(), 1.0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	context->Map(mLightConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &mLightConstBufferData, sizeof(LightConstBufferData));

	context->Unmap(mLightConstBuffer.Get(), 0);
}

void SimpleEngine::PointLightComponent::InitDepthStencilStates(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	D3D11_DEPTH_STENCIL_DESC stateDesc = {};
	stateDesc.DepthEnable = false;
	stateDesc.StencilEnable = false;

	device->CreateDepthStencilState(&stateDesc, mDepthStencilStateOff.GetAddressOf());

	stateDesc.DepthEnable = true;
	stateDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	stateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	device->CreateDepthStencilState(&stateDesc, mDepthStencilStateGreater.GetAddressOf());

	stateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	device->CreateDepthStencilState(&stateDesc, mDepthStencilStateLess.GetAddressOf());
}

void SimpleEngine::PointLightComponent::InitObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device)
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

void SimpleEngine::PointLightComponent::UpdateObjectConstBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	auto transform = GetWorldTransform();
	transform.SetScale(Vector3::One * mLightRadius);
	mObjectConstBufferData.mWorld = transform.GetWorld();
	auto transpInv = transform.GetInvWorld().Transpose();
	mObjectConstBufferData.mTranspInvWorld = transpInv;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(mObjectConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &mObjectConstBufferData, sizeof(ObjectConstBufferData));

	context->Unmap(mObjectConstBuffer.Get(), 0);
}

void SimpleEngine::PointLightComponent::InitVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<Vertex>& vertices)
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

void SimpleEngine::PointLightComponent::InitIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::vector<uint32_t>& indices)
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

namespace SimpleEngine {
	void Subdivide(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices);
	SimpleEngine::Vertex MidPoint(const SimpleEngine::Vertex& v0, const SimpleEngine::Vertex& v1);
}

SimpleEngine::Vertex SimpleEngine::MidPoint(const Vertex& v0, const Vertex& v1)
{
	Vector3 p0 = v0.mPosition;
	Vector3 p1 = v1.mPosition;

	Vector3 n0 = v0.mNormal;
	Vector3 n1 = v1.mNormal;

	Vector3 tan0 = v0.mTangent;
	Vector3 tan1 = v1.mTangent;

	Vector2 tex0 = v0.mUV;
	Vector2 tex1 = v1.mUV;

	// Compute the midpoints of all the attributes.  Vectors need to be normalized
	// since linear interpolating can make them not unit length.  
	Vector3 pos = 0.5f * (p0 + p1);
	Vector3 normal = 0.5f * (n0 + n1);
	normal.Normalize();
	Vector3 tangent = 0.5f * (tan0 + tan1);
	tangent.Normalize();
	Vector2 tex = 0.5f * (tex0 + tex1);

	
	return Vertex(pos, tex, normal, tangent);
}

void SimpleEngine::Subdivide(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices)
{
	// Save a copy of the input geometry.
	auto verticesCopy = vertices;
	auto indicesCopy = indices;


	vertices.resize(0);
	indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	uint32_t numTris = static_cast<uint32_t>(indicesCopy.size() / 3);
	for (uint32_t i = 0; i < numTris; ++i)
	{
		Vertex v0 = verticesCopy[indicesCopy[i * 3 + 0]];
		Vertex v1 = verticesCopy[indicesCopy[i * 3 + 1]];
		Vertex v2 = verticesCopy[indicesCopy[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex m0 = MidPoint(v0, v1);
		Vertex m1 = MidPoint(v1, v2);
		Vertex m2 = MidPoint(v0, v2);

		//
		// Add new geometry.
		//

		vertices.push_back(v0); // 0
		vertices.push_back(v1); // 1
		vertices.push_back(v2); // 2
		vertices.push_back(m0); // 3
		vertices.push_back(m1); // 4
		vertices.push_back(m2); // 5

		indices.push_back(i * 6 + 0);
		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 5);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 2);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 1);
		indices.push_back(i * 6 + 4);
	}
}

void SimpleEngine::PointLightComponent::CreateGeospherevertices(std::vector<SimpleEngine::Vertex>& vertices, std::vector<uint32_t>& indices)
{

	// Approximate a sphere by tessellating an icosahedron.
	constexpr float X = 0.525731f;
	constexpr float Z = 0.850651f;
	constexpr Vector3 pos[12] =
	{
	Vector3(-X, 0.0f, Z), Vector3(X, 0.0f, Z),
	Vector3(-X, 0.0f, -Z), Vector3(X, 0.0f, -Z),
	Vector3(0.0f, Z, X), Vector3(0.0f, Z, -X),
	Vector3(0.0f, -Z, X), Vector3(0.0f, -Z, -X),
	Vector3(Z, X, 0.0f), Vector3(-Z, X, 0.0f),
	Vector3(Z, -X, 0.0f), Vector3(-Z, -X, 0.0f)
	};

	uint32_t k[60] =
	{
	1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4,
	1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2,
	3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
	10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
	};
	vertices.resize(12);
	indices.assign(&k[0], &k[60]);
	for (size_t i = 0; i < 12; ++i)
		vertices[i].mPosition = pos[i];
	
	for (size_t i = 0; i < kNumberGeosphereSubdivision; ++i)
		Subdivide(vertices, indices);
	// Project vertices onto sphere and scale.
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		// Project onto unit sphere.
		Vector3 normal = vertices[i].mPosition;
		normal.Normalize();
		vertices[i].mPosition = normal;
		vertices[i].mNormal = normal;

		// Derive texture coordinates from spherical coordinates.
		// Derive texture coordinates from spherical coordinates.
		float theta = atan2f(vertices[i].mPosition.z, vertices[i].mPosition.x);

		// Put in [0, 2pi].
		if (theta < 0.0f)
			theta += DirectX::XM_2PI;

		float phi = acosf(vertices[i].mPosition.y);

		vertices[i].mUV.x = theta / DirectX::XM_2PI;
		vertices[i].mUV.y = phi / DirectX::XM_PI;

		// Partial derivative of P with respect to theta
		vertices[i].mTangent.x = -sinf(phi) * sinf(theta);
		vertices[i].mTangent.y = 0.0f;
		vertices[i].mTangent.z = +sinf(phi) * cosf(theta);
		vertices[i].mTangent.Normalize();
	}

	///NOTE somehow it is generating triangle in reverce order \_(ツ)_/
	std::reverse(indices.begin(), indices.end());
}