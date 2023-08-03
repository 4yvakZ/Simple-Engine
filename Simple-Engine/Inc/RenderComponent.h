#pragma once

#include "Component.h"

namespace SimpleEngine
{
	class Material;
	struct VertexData;

	class RenderComponent : public Component
	{
	public:
		RenderComponent(std::shared_ptr<Material> material);

		// Inherited via Component
		virtual void init() override;

		virtual void update() override;

		virtual void draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) = 0;

	protected:
		void initVertexandIndexBuffer(const std::vector<VertexData>& vertecis, const std::vector<uint32_t>& indecis);

	protected:
		std::shared_ptr<Material> mMaterial;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	};
}

