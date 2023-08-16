#pragma once
#include "RenderComponent.h"
#include "ShaderStructs.h"

namespace SimpleEngine 
{

	class SpriteComponent : public RenderComponent
	{
	public:
		SpriteComponent(std::shared_ptr<Material> material);
		// Inherited via RenderComponent
		virtual void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) override;
		virtual void Init() override;

	private:
		std::vector<VertexData> mVertecis;
		std::vector<uint32_t> mIndecis;
	};
}

