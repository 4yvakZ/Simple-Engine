#pragma once

#include "SceneComponent.h"
#include "Transform.h"

namespace SimpleEngine
{
	class Material;
	struct VertexData;

	class RenderComponent : public SceneComponent
	{
	public:
		explicit RenderComponent(std::shared_ptr<Material> material);

		// Inherited via Component
		virtual void init() override;

		virtual void update() override;

		virtual void draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) = 0;

		virtual void construct() override;

		auto getMaterial() const
		{
			return mMaterial;
		}

		void setMaterial(std::shared_ptr<Material> material)
		{
			mMaterial = material;
		}

	protected:
		void initVertexandIndexBuffer(const std::vector<VertexData>& vertecis, const std::vector<uint32_t>& indecis);

		auto getIndexBuffer() const
		{
			return mIndexBuffer;
		}
		
		auto getVertexBuffer() const
		{
			return mVertexBuffer;
		}
	private:
		std::shared_ptr<Material> mMaterial;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	};
}

