#include "pch.h"
#include "ShadowMap.h"
#include "AssetManager.h"
#include "Material.h"
#include "Game.h"
#include "CameraComponent.h"
#include "DirectionalLightComponent.h"

using namespace DirectX::SimpleMath;

constexpr float kFarPlaneFraction[] = { 0.05, 0.15, 0.3, 1.0 };
constexpr float kNearPlaneFraction[] = { 0.0, 0.05, 0.15, 0.0 };

const std::string kDefaultShadowMapGSName = "../shaders/DefaultShadowMapGS.hlsl";
const std::string kDefaultShadowMapVSName = "../shaders/DefaultShadowMapVS.hlsl";

SimpleEngine::ShadowMap::ShadowMap(std::shared_ptr<DirectionalLightComponent> dirLight, int width) :
	mWidth(width), mDirectionalLight(dirLight)
{
}

void SimpleEngine::ShadowMap::Init(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	InitCascadeBuffer(device);
	CreateShaderResources(device);
	CompileShaders(device);
}

void SimpleEngine::ShadowMap::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->PSSetConstantBuffers(3, 1, mCascadeBuffer.GetAddressOf());
	context->PSSetShaderResources(5, 1, mShadowMapSRV.GetAddressOf());
	context->PSSetSamplers(1, 1, mSamplerState.GetAddressOf());
}

void SimpleEngine::ShadowMap::SetAsTarget(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	UpdateCascadeBuffer(context);

	ID3D11ShaderResourceView* nullsrv[] = {nullptr};
	context->PSSetShaderResources(5, 1, nullsrv);

	context->RSSetState(mRastState.Get());
	ID3D11RenderTargetView* nullrtv[] = { nullptr };
	context->OMSetRenderTargets(1, nullrtv, mDepthView.Get());
	context->RSSetViewports(1, mViewport.Get11());

	context->PSSetShader(nullptr, nullptr, 0);
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->GSSetShader(mGeometryShader.Get(), nullptr, 0);
	context->GSSetConstantBuffers(3, 1, mCascadeBuffer.GetAddressOf());
}

void SimpleEngine::ShadowMap::Clear(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void SimpleEngine::ShadowMap::CompileShaders(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	D3D_SHADER_MACRO shaderMacros[] = { nullptr, nullptr };
	ID3DBlob* errorVertexCode = nullptr;
	std::wstring VSFileName(kDefaultShadowMapVSName.begin(), kDefaultShadowMapVSName.end());
	auto res = D3DCompileFromFile(VSFileName.c_str(),
		shaderMacros,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		mVertexShaderByteCode.GetAddressOf(),
		&errorVertexCode);
	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << kDefaultShadowMapVSName << std::endl;
		}

		return;
	}

	///pixelShaderByteCode initialization
	std::wstring PSFileName(kDefaultShadowMapGSName.begin(), kDefaultShadowMapGSName.end());
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(PSFileName.c_str(),
		shaderMacros,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"gs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		mGeometryShaderByteCode.GetAddressOf(),
		&errorPixelCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::cout << "Missing Shader File: " << kDefaultShadowMapGSName << std::endl;
		}

		return;
	}

	///vertexShader initialization
	device->CreateVertexShader(
		mVertexShaderByteCode->GetBufferPointer(),
		mVertexShaderByteCode->GetBufferSize(),
		nullptr, mVertexShader.GetAddressOf());

	///pixelShader initialization*/
	device->CreateGeometryShader(
		mGeometryShaderByteCode->GetBufferPointer(),
		mGeometryShaderByteCode->GetBufferSize(),
		nullptr, mGeometryShader.GetAddressOf());
}

void SimpleEngine::ShadowMap::CreateShaderResources(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	D3D11_TEXTURE2D_DESC shadowMapDesc = {};
	shadowMapDesc.ArraySize = 4;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	shadowMapDesc.CPUAccessFlags = 0;
	shadowMapDesc.MiscFlags = 0;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowMapDesc.Width = mWidth;
	shadowMapDesc.Height = mWidth;
	shadowMapDesc.SampleDesc = { 1, 0 };

	device->CreateTexture2D(&shadowMapDesc, nullptr, mShadowMap.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1;
	shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
	shaderResourceViewDesc.Texture2DArray.ArraySize = 4;


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthStencilViewDesc.Texture2DArray.MipSlice = 0;
	depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
	depthStencilViewDesc.Texture2DArray.ArraySize = 4;

	device->CreateShaderResourceView(mShadowMap.Get(), &shaderResourceViewDesc, mShadowMapSRV.GetAddressOf());
	device->CreateDepthStencilView(mShadowMap.Get(), &depthStencilViewDesc, mDepthView.GetAddressOf());

	D3D11_SAMPLER_DESC comparisonSamplerDesc;
	ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	comparisonSamplerDesc.BorderColor[0] = 1.0f;
	comparisonSamplerDesc.BorderColor[1] = 1.0f;
	comparisonSamplerDesc.BorderColor[2] = 1.0f;
	comparisonSamplerDesc.BorderColor[3] = 1.0f;
	comparisonSamplerDesc.MinLOD = 0.f;
	comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	comparisonSamplerDesc.MipLODBias = 0.f;
	comparisonSamplerDesc.MaxAnisotropy = 0;
	comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

	device->CreateSamplerState(&comparisonSamplerDesc, mSamplerState.GetAddressOf());

	mViewport = Viewport(0.0f, 0.0f, static_cast<float>(mWidth), static_cast<float>(mWidth));

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.DepthBias = 0.05;
	rastDesc.DepthBiasClamp = 0.1;
	rastDesc.SlopeScaledDepthBias = 4;

	device->CreateRasterizerState(&rastDesc, mRastState.GetAddressOf());
}

void SimpleEngine::ShadowMap::InitCascadeBuffer(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	///const buffer initialization
	D3D11_BUFFER_DESC cascadeBufDesc = {};
	cascadeBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	cascadeBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cascadeBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cascadeBufDesc.MiscFlags = 0;
	cascadeBufDesc.StructureByteStride = 0;
	cascadeBufDesc.ByteWidth = sizeof(CascadeData);

	D3D11_SUBRESOURCE_DATA cascadeBufData = {};
	cascadeBufData.pSysMem = &mCascadeData;
	cascadeBufData.SysMemPitch = 0;
	cascadeBufData.SysMemSlicePitch = 0;

	device->CreateBuffer(&cascadeBufDesc, &cascadeBufData, mCascadeBuffer.GetAddressOf());
}

void SimpleEngine::ShadowMap::UpdateCascadeBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	auto camera = Game::GetInstance()->GetActiveCameraComponent();
	assert(("No active camera component found", camera));
	auto dirLight = mDirectionalLight.lock();
	assert(("ShadowMap without light source", dirLight));

	for (size_t i = 0; i < kNumberOfCascades; i++)
	{
		Matrix projection;
		float farPlane = camera->GetFarPlane();
		float nearPlane = camera->GetNearPlane();
		mCascadeData.mDistances[i] = (farPlane - nearPlane) * kFarPlaneFraction[i] + nearPlane;
		nearPlane = nearPlane + (farPlane - nearPlane) * kNearPlaneFraction[i];
		
		auto corners = GetFrustrumCornersWorldSpace(camera->GetView(), camera->CreateProjection(nearPlane, mCascadeData.mDistances[i]));

		Vector3 center = Vector3::Zero;
		for (const auto& v : corners)
		{
			center += Vector3(v.x, v.y, v.z);
		}
		center /= corners.size();

		auto view = Matrix::CreateLookAt(center, center + dirLight->GetLightDirection(), Vector3::Up);

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::lowest();
		for (const auto& v : corners)
		{
			auto trf = Vector4::Transform(v, view);
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		constexpr float zMult = 10.0f;

		/*std::cout << minX << " " << maxX << " "
			<< minY << " " << maxY << " "
			<< minZ << " " << maxZ << "\n";*/
			//std::cout << minZ << " " << maxZ << "\n";
		minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
		maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;
		mCascadeData.mViewProjection[i] = view * Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);
	}

	///const buffer update
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto res = context->Map(mCascadeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	memcpy(mappedResource.pData, &mCascadeData, sizeof(CascadeData));

	context->Unmap(mCascadeBuffer.Get(), 0);
}

std::vector<DirectX::SimpleMath::Vector4> SimpleEngine::ShadowMap::GetFrustrumCornersWorldSpace(const Matrix& view, const Matrix& projection)
{
	const auto inv = (view * projection).Invert();

	std::vector<Vector4> frustrumCorners;
	frustrumCorners.reserve(8);
	for (size_t x = 0; x < 2; x++)
	{
		for (size_t y = 0; y < 2; y++)
		{
			for (size_t z = 0; z < 2; z++)
			{
				const Vector4 pt = Vector4::Transform(
					Vector4(2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						z,
						1.0f), inv);
				frustrumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustrumCorners;
}
