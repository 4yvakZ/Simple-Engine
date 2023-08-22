#pragma once
#include "Component.h"
#include "ShaderStructs.h"

namespace SimpleEngine 
{
	class Material;

	class DirectionalLightComponent : public Component
	{
	public:
		DirectionalLightComponent();
		// Inherited via LightComponentBase
		void Init() override;
		void Update() override;
		void Construct() override;
		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

		DirectX::SimpleMath::Vector4 GetLightDirection() const;
		void SetLightDirection(DirectX::SimpleMath::Vector4 lightDirection);

		DirectX::SimpleMath::Vector4 GetLightIntensity() const;
		void SetLightIntensity(DirectX::SimpleMath::Vector4 lightIntensity);

		std::shared_ptr<Material> GetMaterial() const;
		void SetMaterial(std::shared_ptr<Material> material);

	private:
		void InitLightConstBuffer();
		void UpdateLightConstBuffer();
	private:
		LightConstBufferData mLightConstBufferData;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mLightConstBuffer;

		std::shared_ptr<Material> mMaterial;
	};
}

