#pragma once
#include "RenderComponent.h"

namespace SimpleEngine
{
	struct Mesh;

	class MeshComponent : public RenderComponent
	{
	public:
		explicit MeshComponent(std::shared_ptr<Material> material, std::shared_ptr<Mesh> mesh);
		// Inherited via RenderComponent
		void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) override;
		void Init() override;
		
	private:
		std::shared_ptr<Mesh> mMesh;
	};
}