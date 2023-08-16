#pragma once

#include "SceneComponent.h"
#include "ShaderStructs.h"

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

		std::shared_ptr<Material> getMaterial() const;
		void setMaterial(std::shared_ptr<Material> material);

	protected:
		void initVertexBuffer(const std::vector<VertexData>& vertecis);
		void initIndexBuffer(const std::vector<uint32_t>& indecis);
		Microsoft::WRL::ComPtr<ID3D11Buffer> getIndexBuffer() const;
		Microsoft::WRL::ComPtr<ID3D11Buffer> getVertexBuffer() const;

		void initObjectConstBuffer();
		void updateObjectConstBuffer();
		Microsoft::WRL::ComPtr<ID3D11Buffer> getObjectConstBuffer() const;
	private:
		std::shared_ptr<Material> mMaterial;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		ObjectConstBufferData mObjectConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mObjectConstBuffer;
	};
}

