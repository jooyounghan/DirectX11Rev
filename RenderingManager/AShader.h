#pragma once
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#include <vector>

#if defined(_DEBUG)
constexpr UINT ShaderCompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
constexpr UINT ShaderCompileFlag = NULL;
#endif

class AShader
{
public:
	void CreateShader(
		const std::wstring& shaderPath,  
		const std::string& entryPoint, 
		const std::string& targetVersion, 
		ID3D11Device* device
	);

protected:
	virtual void CreateShaderImpl(
		const std::wstring& shaderPath,
		const std::string& entryPoint,
		const std::string& targetVersion,
		ID3D11Device* device
	) = 0;

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> shaderByteCode;
	Microsoft::WRL::ComPtr<ID3DBlob> errorByteCode;

protected:
	std::vector<ID3D11SamplerState*> m_resetSamplerState;

public:
	void ResetByteCode();

public:
	virtual void SetShader(
		ID3D11DeviceContext* deviceContext
	) const = 0;
	virtual void SetSamplerState(
		ID3D11DeviceContext* deviceContext,
		const std::vector<ID3D11SamplerState*>& samplerStates
	);
	virtual void ResetShader(ID3D11DeviceContext* deviceContext) const = 0;
	virtual void ResetSamplerState(ID3D11DeviceContext* deviceContext) const = 0;
};

