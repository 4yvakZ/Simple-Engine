#pragma once
#include "RenderComponent.h"

namespace SimpleEngine
{
	class Mesh;

	class MeshComponent : public RenderComponent
	{
	public:
		MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh);
		// Inherited via RenderComponent
		void draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) override;
		void init() override;
		
	private:
		std::shared_ptr<Mesh> mMesh;
	};
}