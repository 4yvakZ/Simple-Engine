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
		RenderComponent();
		explicit RenderComponent(std::shared_ptr<Material> material);

		// Inherited via Component
		virtual void Init() override;

		virtual void Update() override;

		virtual void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) = 0;

		virtual void Construct() override;

		std::shared_ptr<Material> GetMaterial() const;
		void SetMaterial(std::shared_ptr<Material> material);

	protected:
		void InitVertexBuffer(const std::vector<VertexData>& vertecis);
		void InitIndexBuffer(const std::vector<uint32_t>& indecis);
		Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer() const;
		Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer() const;

		void InitObjectConstBuffer();
		void UpdateObjectConstBuffer();
		Microsoft::WRL::ComPtr<ID3D11Buffer> GetObjectConstBuffer() const;
	private:
		std::shared_ptr<Material> mMaterial;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		ObjectConstBufferData mObjectConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mObjectConstBuffer;
	};
}

