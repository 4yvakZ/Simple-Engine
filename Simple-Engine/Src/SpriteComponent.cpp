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
	UINT strides[] = { sizeof(VertexData) };
	UINT offsets[] = { 0 };

	getMaterial()->bind(context);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->IASetIndexBuffer(getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, getVertexBuffer().GetAddressOf(), strides, offsets);

	/*context->VSSetConstantBuffers(0, 1, &constBuffer);
	context->PSSetConstantBuffers(0, 1, &constBuffer);*/

	context->DrawIndexed(static_cast<UINT>(mIndecis.size()), 0, 0);
}

void SimpleEngine::SpriteComponent::init()
{
	RenderComponent::init();

	mVertecis.reserve(4);

	VertexData point(Vector3 (-0.5, -0.5, 0.5));
	mVertecis.push_back(point);
	point.mPos.y = 0.5;
	mVertecis.push_back(point);
	point.mPos.y = -0.5;
	point.mPos.x = 0.5;
	mVertecis.push_back(point);
	point.mPos.y = 0.5;
	mVertecis.push_back(point);

	initVertexBuffer(mVertecis);
	initIndexBuffer(mIndecis);
}
