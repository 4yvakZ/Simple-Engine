#include "pch.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Material.h"

SimpleEngine::MeshComponent::MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh):
	RenderComponent(material),
	mMesh(mesh)
{
}

void SimpleEngine::MeshComponent::draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	///Setup AI stage
	UINT strides[] = { 48 };
	UINT offsets[] = { 0 };

	getMaterial()->bind(context);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, getVertexBuffer().GetAddressOf(), strides, offsets);

	/*context->VSSetConstantBuffers(0, 1, &constBuffer);
	context->PSSetConstantBuffers(0, 1, &constBuffer);*/

	context->DrawIndexed(static_cast<UINT>(mMesh->getIndecis().size()), 0, 0);
}

void SimpleEngine::MeshComponent::init()
{
	RenderComponent::init();

	initVertexandIndexBuffer(mMesh->getVertecis(), mMesh->getIndecis());
}
