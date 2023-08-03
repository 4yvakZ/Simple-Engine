#include "pch.h"
#include "SpriteComponent.h"
#include "ShaderStructs.h"
#include "Material.h"

using namespace DirectX::SimpleMath;

SimpleEngine::SpriteComponent::SpriteComponent(std::shared_ptr<Material> material) :
	RenderComponent(material),
	mVertecis(),
	mIndecis{0, 1, 2, 3}
{
}

void SimpleEngine::SpriteComponent::draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	///Setup AI stage
	UINT strides[] = { 48 };
	UINT offsets[] = { 0 };

	mMaterial->bind(context);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);

	/*context->VSSetConstantBuffers(0, 1, &constBuffer);
	context->PSSetConstantBuffers(0, 1, &constBuffer);*/

	context->DrawIndexed(mIndecis.size(), 0, 0);
}

void SimpleEngine::SpriteComponent::init()
{
	RenderComponent::init();

	mVertecis.reserve(4);

	VertexData point(Vector3 (-0.5, -0.5, 0.5), Color(1,1,1,1));
	mVertecis.push_back(point);
	point.mPos.y = 0.5;
	mVertecis.push_back(point);
	point.mPos.y = -0.5;
	point.mPos.x = 0.5;
	mVertecis.push_back(point);
	point.mPos.y = 0.5;
	mVertecis.push_back(point);

	initVertexandIndexBuffer(mVertecis, mIndecis);
}
