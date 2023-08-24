#include "pch.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Material.h"

SimpleEngine::MeshComponent::MeshComponent(std::shared_ptr<Mesh> mesh):
	RenderComponent(),
	mMesh(mesh)
{
}

SimpleEngine::MeshComponent::MeshComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material):
	RenderComponent(material),
	mMesh(mesh)
{
}

void SimpleEngine::MeshComponent::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	///Setup AI stage
	UINT strides[] = { sizeof(VertexData) };
	UINT offsets[] = { 0 };

	//GetMaterial()->Bind(context);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, GetVertexBuffer().GetAddressOf(), strides, offsets);

	context->VSSetConstantBuffers(1, 1, GetObjectConstBuffer().GetAddressOf());
	context->PSSetConstantBuffers(1, 1, GetObjectConstBuffer().GetAddressOf());

	context->DrawIndexed(static_cast<UINT>(mMesh->GetIndecis().size()), 0, 0);
}

void SimpleEngine::MeshComponent::Init()
{
	RenderComponent::Init();

	InitVertexBuffer(mMesh->GetVertecis());
	InitIndexBuffer(mMesh->GetIndecis());
}
