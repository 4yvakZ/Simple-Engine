#include "pch.h"
#include "Material.h"

#include <DirectXTex.h>

const std::string SimpleEngine::Material::kDefaultPSName = "../shaders/DefaultPS.hlsl";
const std::string SimpleEngine::Material::kDefaultVSName = "../shaders/DefaultVS.hlsl";

SimpleEngine::Material::Material():
	mPSFileName(kDefaultPSName),
	mVSFileName(kDefaultVSName)
{
}

void SimpleEngine::Material::init(Microsoft::WRL::ComPtr<ID3D11Device> device)
{

	D3D_SHADER_MACRO shaderMacros[] = {nullptr, nullptr };

	ID3DBlob* errorVertexCode = nullptr;
	std::wstring VSFileName(mVSFileName.begin(), mVSFileName.end());
	auto res = D3DCompileFromFile(VSFileName.c_str(),
		shaderMacros /*macros*/,
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
			std::cout << "Missing Shader File: " << mVSFileName << std::endl;
		}

		return;
	}

	///pixelShaderByteCode initialization
	std::wstring PSFileName(mPSFileName.begin(), mPSFileName.end());
	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(PSFileName.c_str(),
		shaderMacros /*macros*/,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		mPixelShaderByteCode.GetAddressOf(),
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
			std::cout << "Missing Shader File: " << mPSFileName << std::endl;
		}

		return;
	}

	///vertexShader initialization
	device->CreateVertexShader(
		mVertexShaderByteCode->GetBufferPointer(),
		mVertexShaderByteCode->GetBufferSize(),
		nullptr, mVertexShader.GetAddressOf());

	///pixelShader initialization
	device->CreatePixelShader(
		mPixelShaderByteCode->GetBufferPointer(),
		mPixelShaderByteCode->GetBufferSize(),
		nullptr, mPixelShader.GetAddressOf());

	///layout initialization
	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	res = device->CreateInputLayout(
		inputElements,
		std::size(inputElements),
		mVertexShaderByteCode->GetBufferPointer(),
		mVertexShaderByteCode->GetBufferSize(),
		mInputLayout.GetAddressOf());

	///rastState initialization
	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	device->CreateRasterizerState(&rastDesc, mRastState.GetAddressOf());

	/*
	std::wstring fileName(mTextureName.begin(), mTextureName.end());
	DirectX::ScratchImage image;
	HRESULT hr;
	if (_wcsicmp(fileName.c_str(), L".dds") == 0)
	{
		hr = LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".tga") == 0)
	{
		hr = LoadFromTGAFile(fileName.c_str(), nullptr, image);
	}
	else if (_wcsicmp(fileName.c_str(), L".hdr") == 0)
	{
		hr = LoadFromHDRFile(fileName.c_str(), nullptr, image);
	}
	else
	{
		hr = LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_DEFAULT_SRGB, nullptr, image);
	}

	if (SUCCEEDED(hr))
	{
		hr = CreateShaderResourceView(device.Get(),
			image.GetImages(), image.GetImageCount(),
			image.GetMetadata(), &mTextureView);
	}
	else
	{
		//TODO Add default debug texture 
		std::cout << "Missing texture " << mTextureName << "\n";
	}

	///samplerState initialization

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MaxLOD = INT_MAX;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 16;

	hr = device->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf());
	*/
}

void SimpleEngine::Material::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->RSSetState(mRastState.Get());
	context->IASetInputLayout(mInputLayout.Get());

	///Set Vertex and Pixel Resources
	context->VSSetShader(mVertexShader.Get(), nullptr, 0);
	context->PSSetShader(mPixelShader.Get(), nullptr, 0);
	/*	
	context->PSSetShaderResources(0, 1, &textureView);
	context->PSSetSamplers(0, 1, &samplerState);

	context->PSSetConstantBuffers(1, 1, &materialBuffer);*/
}

void SimpleEngine::Material::setPSFileName(const std::string& PSFileName)
{
	mPSFileName = PSFileName;
}

std::string SimpleEngine::Material::PSFileName() const
{
	return mPSFileName;
}

void SimpleEngine::Material::setVSFileName(const std::string& VSFileName)
{
	mVSFileName = VSFileName;
}

std::string SimpleEngine::Material::VSFileName() const
{
	return mVSFileName;
}
